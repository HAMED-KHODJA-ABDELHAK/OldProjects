package drs.client;

import java.io.IOException;
import java.net.SocketException;
import java.rmi.NotBoundException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import drs.DRServer;
import drs.TransactionResult;

/**
 * Top class for test cases.
 */
public class ClientBuyItems extends ClientBase {
	private int itemID;
	private int numItems;

	public ClientBuyItems(String cID, int itemID, int numItems, int delay) {
		super(cID, delay);

		this.itemID = itemID;
		this.numItems = numItems;
	}

	@Override
	public void procedure() {
		try {
            Registry registry = LocateRegistry.getRegistry();
            DRServer drs = (DRServer)registry.lookup(determineServerName() + DRServer.RETURN_ITEM);

            TransactionResult result;
            result = drs.buyItems(cID, itemID, numItems);

            if (result.suceeded()) {
	            printWithID(result.getData());
	            printWithID("Finished client work");
            } else {
            	printWithID("Failed first time, trying again after random wait.");
            	Thread.sleep((long) (5000 * Math.random()));
            	result = drs.buyItems(cID, itemID, numItems);
	            printWithID(result.getData());
	            printWithID("Finished client work");
            }

		} catch (SocketException e) {
			printWithID("Socket: " + e.getMessage());
		} catch (IOException e) {
			printWithID("IO: " + e.getMessage());
		} catch (NotBoundException e) {
			printWithID("RMI Registry error: " + e.getMessage());
		} catch (InterruptedException e) {
			printWithID("Interrupted during semaphore: " + e.getMessage());
		}
	}

	public static void main(String[] args) {
		// Not a real ID, will fail.
		ClientBuyItems c = new ClientBuyItems("M43930", 552, 2, 0);
		c.start();

		// Deducts from first store.
		ClientBuyItems c2 = new ClientBuyItems("M43931", 2000, 2, 0);
		c2.start();

		// Will test recursion, will report number sold.
		ClientBuyItems c3 = new ClientBuyItems("M43932", 2000, 10, 0);
		c3.start();
	}
}
