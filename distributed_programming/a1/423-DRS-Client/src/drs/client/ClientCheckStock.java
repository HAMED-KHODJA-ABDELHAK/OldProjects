package drs.client;

import java.io.IOException;
import java.net.SocketException;
import java.rmi.NotBoundException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Map;

import drs.DRServer;
import drs.StockAvailable;


/**
 * Top class for test cases.
 */
public class ClientCheckStock extends ClientBase {
	private int itemID;

	public ClientCheckStock(String cID, int itemID, int delay) {
		super(cID, delay);

		this.itemID = itemID;
	}

	@Override
	public void procedure() {
		try {
            Registry registry = LocateRegistry.getRegistry();
            DRServer drs = (DRServer)registry.lookup(determineServerName() + DRServer.RETURN_ITEM);

            StockAvailable result;
            result = drs.checkStock(itemID);

            printWithID(String.format("For the item %d the following stores have:\n", itemID));

            Map<Character, Integer> storeStock = result.getStoreMap();

            for (char c : storeStock.keySet())
            	printWithID(String.format("The store %c had %d of the item %d.\n", c, storeStock.get(c), itemID));

            printWithID("Finished client work");

		} catch (SocketException e) {
			printWithID("Socket: " + e.getMessage());
		} catch (IOException e) {
			printWithID("IO: " + e.getMessage());
		} catch (NotBoundException e) {
			printWithID("RMI Registry error: " + e.getMessage());
		}
	}

	public static void main(String[] args) {
		ClientCheckStock c = new ClientCheckStock("M43921", 664, 0);
		c.start();

		ClientCheckStock c2 = new ClientCheckStock("V39211", 2000, 0);
		c2.start();
	}
}