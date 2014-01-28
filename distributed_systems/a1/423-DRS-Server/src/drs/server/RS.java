package drs.server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Set;

import drs.DRServer;
import drs.StockAvailable;
import drs.TransactionResult;
import drs.UDPMessage;

/**
 * Manager class, other threads for RMI and UDP communication will share access to
 * an instance of this class.
 *
 * Inventory access is protected by synchonized methods on those that modify data. The read is NOT protected
 * by design to increase concurrency while sacrificing a little inaccuracy in latest information.
 *
 * Log needs no protection, it doesn't matter if two lines of the log aren't written in order.
 *
 * Servers needs no protection, I'm using a hash set and only ever adding elements or reading current set.
 */
public class RS implements DRServer {
	private static final int MAX_ITEMS = 10;
	private HashMap<Integer, Item> inventory;
	private HashMap<String, Log> logs;
	private HashSet<Character> servers;
	private Character storeName;

	public RS(Character store)  {
		inventory = new HashMap<Integer, Item>();
		logs = new HashMap<String, Log>();
		servers = new HashSet<Character>();
		storeName = store;

		initInventory();
	}

	public RS(Character store, Map<Integer, Item> initInventory) {
		inventory = new HashMap<Integer, Item>(initInventory);
		logs = new HashMap<String, Log>();
		servers = new HashSet<Character>();
		storeName = store;
	}

	public Log getCustomerLog(String cID) throws IOException {
		Log log = logs.get(cID.substring(1));

		if (log == null) {
			log = new Log(storeName.toString(), cID);
			logs.put(cID, log);
		}

		return log;
	}

	public Character getStoreName() {
		return storeName;
	}

	public Item getItem(int itemID) {
		return inventory.get(itemID);
	}

	public synchronized Set<Character> getServers() {
		Set<Character> copy = new HashSet<Character>(servers);
		return copy;
	}

	public synchronized void addServer(Character c) {
		servers.add(c);
	}

	private void initInventory() {
		for (int i = 0; i < MAX_ITEMS; ++i) {
			int id = 1000 + i * 500;
			int stock = i * 2;
			Item item = new Item(id, stock);
			inventory.put(id, item);
		}
	}

	/* Get the next server to call, bit messy could be improved. */
	private char nextServer() {
		LinkedList<Character> list = new LinkedList<Character>(getServers());
		Collections.sort(list);
		boolean returnNext = false;

		for (char c : list) {
			if (returnNext == true)
				return c;

			if (c == storeName)
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

	@Override
	public TransactionResult buyItems(String cID, int itemID, int numItems)
			throws RemoteException, IOException, NotBoundException, InterruptedException {
		System.out.println("Got a request to buy items.");

		Log log = getCustomerLog(cID);
		TransactionResult result = null;
		Item item = inventory.get(itemID);

		/* Lock current state, preventing inventory modification. If can't, request user try again later. */
		if (item != null && item.tryDownSem() == false) {
			result = new TransactionResult("Unable to aquire enough stock at this time, try again later.\n", false);
			return result;
		}

		/* If transaction can be completed here, do it and return. */
		if (item != null && item.getStock() >= numItems) {
			item.buyItems(numItems);
			item.upSem();
			log.buyItems(itemID, numItems);
			result = new TransactionResult(String.format("Purchase succeeded, %s bought %d of %d.",
					cID, numItems, itemID), true);

			return result;
		}

		/* Base case, check if we've gone through all servers. This happens when nextServer = closest from cDI. */
		char nextServer = nextServer();
		if (cID.toUpperCase().charAt(0) == nextServer) {
			if (item != null)
				item.upSem();

			result = new TransactionResult("Unable to aquire enough stock at this time, try again later.\n", false);
			return result;
		}

		/* Still have servers, how many needed in others?. */
		int numRequiredElsewhere = numItems;
		if (item != null && item.getStock() > 0) {
			numRequiredElsewhere -= item.getStock();
		}

		/* Start a recursive RMI call, fails if hits base case above. */
        Registry registry = LocateRegistry.getRegistry();
        DRServer drs = (DRServer)registry.lookup(nextServer + DRServer.CHECK_STOCK);
        result = drs.buyItems(cID, itemID, numRequiredElsewhere);

        /* Propagate false return. */
        if (result.suceeded() == false) {
			if (item != null)
				item.upSem();

			return result;
        }

        /* If gets here, deduct the amount we have if any... */
        if (item != null && item.getStock() > 0) {
        	int numSold = item.getStock();
        	item.buyItems(numSold);
        	item.upSem();
        	log.buyItems(itemID, numSold);

			result = new TransactionResult(String.format("This store sold %d of item %d, the rest came from other stores.\n", numSold, itemID), true);
        	return result;
        }

		result = new TransactionResult(String.format("This store sold no items of item %d, all came from others.\n", itemID), true);
		return result;
	}


	@Override
	public TransactionResult returnItems(String cID, int itemID, int numItems)
			throws RemoteException, IOException, InterruptedException {
		System.out.println("Got a request to return items.");

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


		/* Never fails, stores always accept return items. */
		return new TransactionResult(String.format("Returned: %d of %d to store %s\n", numItems, itemID, ""+storeName), true);
	}

	@Override
	public StockAvailable checkStock(int itemID) throws RemoteException, IOException {
		System.out.println("Got a request to check store stock.");

		StockAvailable sa = new StockAvailable(itemID);

		/* Fill in this stores inventory. */
		Item item = inventory.get(itemID);
		if (item != null) {
			sa.addStore(storeName, item.getStock());
		} else {
			sa.addStore(storeName, 0);
		}

		DatagramSocket socket;

		try {
			socket = new DatagramSocket();

			/* Make a get stock request message. */
			UDPMessage message = new UDPMessage(UDPMessage.CMD_GET_STOCK);
			message.addArg(UDPMessage.ARG_ITEM_ID, ""+itemID);
			message.makeMessage();
			byte[] buf = message.getBytes();

			/* Make an immutable copy of current server list, thread protection. */
			Set<Character> serversCopy = getServers();
			serversCopy.remove(storeName);
			int numResponses = serversCopy.size();

			/* Send request to every other server. */
			for (char c : serversCopy) {
				DatagramPacket packet = new DatagramPacket(buf, buf.length,
						InetAddress.getLocalHost(), UDPMessage.STARTING_PORT + c);
				socket.send(packet);
			}

			/* Wait for all responses. */
			while (numResponses > 0) {
				byte[] buffer = new byte[UDPMessage.MAX_BYTES];
				DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
				socket.receive(packet);

				message = new UDPMessage(packet.getData());
				message.parseBytes();

				String cmd = message.getCmd();

				/* If we get a response packet, add to sa and then decrement counter. */
				if (cmd.equals(UDPMessage.CMD_STOCK_INFO)) {
					String arg = message.getArg(UDPMessage.ARG_ITEM_STOCK);
					int stock = (int) Double.parseDouble(arg); // Odd bug with Integer.parseInt, using double parsing instead.

					arg = message.getArg(UDPMessage.ARG_ITEM_STORE);
					char store = arg.charAt(0);

					sa.addStore(store, stock);
					numResponses--;
				}
			}
		/* If there's any exception, simply throw to client. */
		} catch (Exception e) {
			throw new RemoteException();
		}

		return sa;
	}
}
