package corba.server;

import java.io.File;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;
import java.util.Properties;
import java.util.Scanner;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;

import common.InvFile;

import CORBAServer.CorbaIOException;
import CORBAServer.CorbaInetAddressException;
import CORBAServer.CorbaInterruptedException;
import CORBAServer.CorbaNotEnoughStockException;
import CORBAServer.Result_s;
import CORBAServer.RetailStore;
import CORBAServer.RetailStoreHelper;
import CORBAServer.Stock_s;

import corba.base.DRServer;

/** To be used by the RM to send requests directly into the different stores and get a response.
 *
 */
public class JeremyRetailStoreCollection implements corba.base.RetailStoreCollection {
	private HashMap<String, RetailStore> stubs;

	public JeremyRetailStoreCollection() {
		stubs = new HashMap<String, RetailStore>();
	}

	public void startServers() throws Exception {
		String[] servers = {"M", "T", "V"};

		CentralServer central = new CentralServer();
		central.start();

		// Parse args from config file if not on args.
		Scanner scan = new Scanner(new File(DRServer.ORBD_CONFIG));
		scan.nextLine();
		String argsLine = scan.nextLine();
		String[] orbArgs = argsLine.split(" ");

		// Orb properties to pass at init.
		Properties orbProps = System.getProperties();
		orbProps.setProperty("org.omg.CORBA.ORBClass", "com.sun.corba.se.internal.POA.POAORB");
		orbProps.setProperty("org.omg.CORBA.ORBSingletonClass", "com.sun.corba.se.internal.corba.ORBSingleton");

		// Init the ORB with the args passed.
		ORB orb = ORB.init(orbArgs, orbProps);

		// Get the root POA.
		POA poaRoot = (POA) orb.resolve_initial_references("RootPOA");
		poaRoot.the_POAManager().activate();

		// Get naming context for binding.
		org.omg.CORBA.Object objRef  = orb.resolve_initial_references("NameService");
		NamingContextExt nc = NamingContextExtHelper.narrow(objRef);

		/* For each server, bind the necessary associations in the nc. */
    	for (String sName : servers) {

    		// Get current state from file, unless not existing.
    		InvFile invFile = new InvFile(sName);
    		Map<String, Integer> initInventory = invFile.getMappedState();

    		RetailStoreServerImpl server;
    		// Create server and make interface reference.
    		if (initInventory.size() > 0)
    			server = new RetailStoreServerImpl(sName, initInventory);
    		else
    			server = new RetailStoreServerImpl(sName);

    		server.setOrb(orb);
			objRef = poaRoot.servant_to_reference(server);
			RetailStore servRef = RetailStoreHelper.narrow(objRef);
			stubs.put(sName, servRef);

			NameComponent path[] = nc.to_name(sName);
			nc.rebind(path, servRef);

        	RetailStoreUDP udp = new RetailStoreUDP(server);
        	udp.start();
    	}

		System.out.println("CORBA Server ready...");
	}

	private RetailStore getStub(String customerID) {
		String serverToContact = customerID.substring(0,1);
		return stubs.get(serverToContact);
	}

	private RetailStore getStub() {
		return stubs.values().iterator().next();
	}

	@Override
	public Boolean BuyItems(String customerID, String itemID, int numberOfItems) {
		Result_s result = null;
		RetailStore target = getStub(customerID);
		int pItemID = (int) Double.parseDouble(itemID);

		try {
			result = target.buyItems(customerID, pItemID, numberOfItems, customerID.substring(0, 1));
		} catch (CorbaIOException e) {
			e.printStackTrace();
		} catch (CorbaInterruptedException e) {
			e.printStackTrace();
		} catch (CorbaNotEnoughStockException e) {
			e.printStackTrace();
		}

		if (result == null || result.success == false)
			return false;
		else
			return true;
	}

	@Override
	public Boolean ReturnItems(String customerID, String itemID,
			int numberOfItems) {
		Result_s result = null;
		RetailStore target = getStub(customerID);
		int pItemID = (int) Double.parseDouble(itemID);

		try {
			result = target.returnItems(customerID, pItemID, numberOfItems);
		} catch (CorbaIOException e) {
			e.printStackTrace();
		} catch (CorbaInterruptedException e) {
			e.printStackTrace();
		}

		if (result == null || result.success == false)
			return false;
		else
			return true;
	}

	@Override
	public Boolean ExchangeItems(String customerID, String itemID,
			int numberOfItems, String newItemID, int newNumberOfItems) {
		Result_s result = null;
		RetailStore target = getStub(customerID);
		int pItemID = (int) Double.parseDouble(itemID), pNewItemID = (int) Double.parseDouble(itemID);

		try {
			result = target.exchange(customerID, pItemID, numberOfItems, pNewItemID, newNumberOfItems);
		} catch (CorbaIOException e) {
			e.printStackTrace();
		} catch (CorbaInterruptedException e) {
			e.printStackTrace();
		} catch (CorbaNotEnoughStockException e) {
			e.printStackTrace();
		}

		if (result == null || result.success == false)
			return false;
		else
			return true;
	}

	@Override
	public Hashtable<Character, Integer> CheckStockItems(String itemID) {
		Stock_s[] result = null;
		Hashtable<Character, Integer> stock = new Hashtable<Character, Integer>();
		RetailStore target = getStub();
		int pItemID = (int) Double.parseDouble(itemID);

		try {
			result = target.checkStock(pItemID);
		} catch (CorbaIOException e) {
			e.printStackTrace();
		} catch (CorbaInetAddressException e) {
			e.printStackTrace();
		}

		if (result != null)
			for (Stock_s e : result)
				stock.put(e.storeName.charAt(0), e.num);

		return stock;
	}

	public static void main(String[] args) {
		JeremyRetailStoreCollection d = new JeremyRetailStoreCollection();
		try {
			d.startServers();
			System.out.println(d.ReturnItems("M49221", "3000", 2));
			Map<Character,Integer> result = d.CheckStockItems("3000");
			for (Character c : result.keySet()) {
				System.out.println(String.format("Key:%s,Value:%s", c, result.get(c)));
			}
		} catch (Exception e) {
		}

	}
}
