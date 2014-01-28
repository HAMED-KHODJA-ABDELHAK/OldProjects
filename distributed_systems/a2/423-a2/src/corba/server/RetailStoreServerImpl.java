package corba.server;


import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Set;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;

import common.InvFile;

import corba.base.SendMessage;
import corba.base.ServerAddress;
import corba.base.StockAvailable;
import corba.base.Message;

import CORBAServer.CorbaIOException;
import CORBAServer.CorbaInterruptedException;
import CORBAServer.CorbaNotEnoughStockException;
import CORBAServer.Result_s;
import CORBAServer.Stock_s;

/**
 * This class is the implemetation object for your IDL interface.
 *
 * Let the Eclipse complete operations code by choosing 'Add unimplemented methods'.
 */
public class RetailStoreServerImpl extends CORBAServer.RetailStorePOA {
	private static final int MAX_ITEMS = 10;
	private HashMap<Integer, Item> inventory;
	private HashMap<String, Log> logs;
	private HashMap<String, ServerAddress> addrDB;
	private String storeName;
	private org.omg.CORBA.ORB orb = null;

	public RetailStoreServerImpl(String store)  {
		inventory = new HashMap<Integer, Item>();
		logs = new HashMap<String, Log>();
		addrDB = new HashMap<String, ServerAddress>();
		storeName = store;

		initInventory();
	}

	/* Map here should come from invFile. */
	public RetailStoreServerImpl(String store, Map<String, Integer> initInventory) {
		inventory = new HashMap<Integer, Item>();
		logs = new HashMap<String, Log>();
		addrDB = new HashMap<String, ServerAddress>();
		storeName = store;

		for (String key : initInventory.keySet()) {
			int itemID = (int) Double.parseDouble(key);
			Item item = new Item(itemID, initInventory.get(key));
			inventory.put(itemID, item);
		}
	}

	private void initInventory() {
		for (int i = 0; i < MAX_ITEMS; ++i) {
			int id = 1000 + i * 500;
			int stock = i * 2;
			Item item = new Item(id, stock);
			inventory.put(id, item);
		}
	}

	private void updateInvFile() throws IOException {
		InvFile invFile = new InvFile(storeName);
		HashMap<String, Integer> invMap = new HashMap<String, Integer>();

		for (Integer key : inventory.keySet()) {
			invMap.put(""+key, inventory.get(key).getStock());
		}

		invFile.writeState(invMap);
	}

	public Log getCustomerLog(String cID) throws FileNotFoundException {
		Log log = logs.get(cID.substring(1));

		if (log == null) {
			log = new Log(storeName.toString(), cID);
			log.openLog();
			logs.put(cID, log);
		}

		return log;
	}

	public String getStoreName() {
		return storeName;
	}

	public Item getItem(int itemID) {
		return inventory.get(itemID);
	}

	public Set<String> getServerKeys() {
		Set<String> copy = new HashSet<String>(addrDB.keySet());
		return copy;
	}

	public Set<ServerAddress> getServerAddresses() {
		Set<ServerAddress> copy = new HashSet<ServerAddress>(addrDB.values());
		return copy;
	}

	public void addServer(ServerAddress sa) {
		addrDB.put(sa.getName(), sa);
	}

	/* Get the next server to call, bit messy could be improved. */
	private String nextServer() {
		LinkedList<String> list = new LinkedList<String>(getServerKeys());
		Collections.sort(list);
		boolean returnNext = false;

		for (String s : list) {
			if (returnNext)
				return s;

			if (s.equals(storeName))
				returnNext = true;
		}

		return list.getFirst();
	}

	public String dumpInventory() {
		String currentInventory = "";

		for (int itemID : inventory.keySet()) {
			Item item = inventory.get(itemID);
			currentInventory += String.format("%d %d\n", itemID, item.getStock());
		}

		return currentInventory;
	}

	public void setOrb(ORB orb) {
			this.orb = orb;
	}

	@Override
	public Result_s buyItems(String cID, int itemID, int numItems, String initStore)
			throws CorbaIOException, CorbaInterruptedException, CorbaNotEnoughStockException {
		try {
			Log log = getCustomerLog(cID);
			Item item = inventory.get(itemID);
			Result_s result = null;

			/* Lock current state, preventing inventory modification. If can't, request user try again later. */
			if (item != null && item.tryDownSem() == false) {
				return new Result_s(false, String.format("Failed to aquire lock, try again shortly."));
			}

			/* If transaction can be completed here, do it and return. */
			if (item != null && item.getStock() >= numItems) {
				item.buyItems(numItems);
				item.upSem();
				log.buyItems(itemID, numItems);
				updateInvFile();

				return new Result_s(true, String.format("Purchase succeeded, %s bought %d of %d.",
								cID, numItems, itemID));
			}

			/* Base case, check if we've gone through all servers. This happens when nextServer = first one called. */
			String nextServer = nextServer();
			if (nextServer.equals(initStore)) {
				if (item != null)
					item.upSem();

				throw new CorbaNotEnoughStockException("Searched all servers, can't fill order.");
			}

			/* Still have servers, how many needed in others?. */
			int numRequiredElsewhere = numItems;
			if (item != null && item.getStock() > 0) {
				numRequiredElsewhere -= item.getStock();
			}

			// Get the naming service.
			org.omg.CORBA.Object ncObj = orb.resolve_initial_references("NameService");
			NamingContextExt nc = NamingContextExtHelper.narrow(ncObj);

			// Resolve the method required to a stub reference.;
			CORBAServer.RetailStore target = CORBAServer.RetailStoreHelper.narrow(nc.resolve_str(nextServer));
			result = target.buyItems(cID, itemID, numRequiredElsewhere, initStore);

	        /* Propagate false return. */
	        if (result.success == false) {
				if (item != null)
					item.upSem();

				return result;
	        }

	        /* If gets here, deduct the amount we have if any... */
	        String storeMessage;

	        if (item != null && item.getStock() > 0) {
	        	int numSold = item.getStock();
	        	item.buyItems(numSold);
	        	item.upSem();
	        	log.buyItems(itemID, numSold);
	        	updateInvFile();

				storeMessage = String.format("This store sold %d of item %d, the rest came from other stores.\n", numSold, itemID);
	        } else {
	        	storeMessage = String.format("This store sold no items of item %d, all came from others.\n", itemID);
	        }

        	return new Result_s(true, storeMessage);
		} catch (FileNotFoundException e) {
			throw new CorbaIOException(e.getMessage());
		} catch (Exception e) {
			throw new CorbaIOException(e.getMessage());
		}
	}

	@Override
	public Result_s returnItems(String cID, int itemID, int numItems)
			throws CorbaIOException, CorbaInterruptedException {
		try {
		Log log = getCustomerLog(cID);
		Item item = inventory.get(itemID);

		if (item == null) {
			item = new Item(itemID, 0);
			inventory.put(itemID, item);
		}

		item.downSem();
		item.returnItems(numItems);
		item.upSem();
		log.returnItems(itemID, numItems);
		updateInvFile();

		/* Never fails, stores always accept return items. */
		return new Result_s(true, String.format("Returned: %d of %d to store %s\n", numItems, itemID, ""+storeName));
		} catch (InterruptedException e) {
			throw new CorbaInterruptedException(e.getMessage());
		} catch (IOException e) {
			throw new CorbaIOException(e.getMessage());
		}
	}

	private int getNumSold(String cID, int itemID) throws IOException {
		int numSold = 0;

		DatagramSocket socket;

		socket = new DatagramSocket();

		/* Make a get stock request message. */
		Message message = Message.numSoldRequest(itemID, cID);

		/* Make an immutable copy of current server list, thread protection. */
		Set<ServerAddress> serverAddrs= getServerAddresses();
		int numResponses = serverAddrs.size();

		/* Send request to every server. */
		SendMessage.toAllServers(socket, message, serverAddrs);

		/* Wait for all responses. */
		while (numResponses > 0) {
			byte[] buffer = new byte[Message.getMaxBytes()];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			socket.receive(packet);

			message = new Message(packet);
			message.parseBytes();

			String cmd = message.getCmd();

			/* If we get a response packet, add to sa and then decrement counter. */
			if (cmd.equals(Message.CMD_NUM_SOLD_REPLY)) {
				int logNumSold = (int) Double.parseDouble(message.getArg(Message.INT_NUM_SOLD));

				numSold += logNumSold;
				numResponses--;
			}
		}

		return numSold;
	}

	@Override
	public Result_s exchange(String cID, int bItemID, int bNumItems,
			int dItemID, int dNumItems) throws CorbaNotEnoughStockException, CorbaIOException,
			CorbaInterruptedException {
		try {
			Log log = getCustomerLog(cID);
			Result_s result;

			// Can only accept as many returns from customer as previously purchased from our stores.
			int numSold = getNumSold(cID, bItemID);
			System.out.println(numSold);
			if (bNumItems > numSold)
				throw new CorbaNotEnoughStockException("Customer did not previously purchase bought items from DRS.");

			// Check current stock snapshot, if we don't have enough abort.
			Stock_s[] stock = checkStock(dItemID);

			int totalStock = 0;
			for (Stock_s s: stock)
				totalStock += s.num;

			if (totalStock < dNumItems)
				throw new CorbaNotEnoughStockException("Not enough stock to purchase in all stores at this time.");

			// Attempt to purchase, if this works, we can accept the return. The recursive buyItems is already atomic.
			// If it fails, we just throw an exception and fail. This might happen if a transaction removes stock post checkStock call.
			result = buyItems(cID, dItemID, dNumItems, getStoreName());

			if (result.success) {
				returnItems(cID, bItemID, bNumItems);
				log.exchangeItems(bItemID, bNumItems, dItemID, dNumItems);
				updateInvFile();
				return new Result_s(true, "Successfully completed the exchange. Have a nice day.");
			}

			return new Result_s(false, "Try again later, temporary difficulties.");

		} catch (IOException e) {
			throw new CorbaIOException(e.getMessage());
		}
	}

	@Override
	public Stock_s[] checkStock(int itemID) throws CorbaIOException {
		StockAvailable sa = new StockAvailable(itemID);

		DatagramSocket socket;

		try {
			socket = new DatagramSocket();

			/* Make a get stock request message. */
			Message message = Message.stockRequest(itemID);

			/* Make an immutable copy of current server list minus this store, thread protection. */
			Set<ServerAddress> serverAddrs = getServerAddresses();
			int numResponses = serverAddrs.size();

			/* Send request to every other server. */
			SendMessage.toAllServers(socket, message, serverAddrs);

			/* Wait for all responses. */
			while (numResponses > 0) {
				byte[] buffer = new byte[Message.getMaxBytes()];
				DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
				socket.receive(packet);

				message = new Message(packet);
				message.parseBytes();

				String cmd = message.getCmd();

				/* If we get a response packet, add to sa and then decrement counter. */
				if (cmd.equals(Message.CMD_GET_STOCK_REPLY)) {
					String arg = message.getArg(Message.INT_ITEM_STOCK);
					int stock = (int) Double.parseDouble(arg); // Odd bug with Integer.parseInt, using double parsing instead.

					String storeName = message.getArg(Message.STR_SERVER_NAME);

					sa.addStore(storeName, stock);
					numResponses--;
				}
			}
		/* If there's any exception, simply throw to client. */
		} catch (IOException e) {
			throw new CorbaIOException(e.getMessage());
		}

		return sa.getArray();
	}
}
