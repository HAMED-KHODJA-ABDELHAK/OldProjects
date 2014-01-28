package drs.client;

import java.io.IOException;
import java.net.SocketException;
import java.rmi.NotBoundException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import drs.DRServer;
import drs.TransactionResult;

public class ClientReturnItems extends ClientBase {
	private int itemID;
	private int numItems;

	public ClientReturnItems(String cID, int itemID, int numItems, int delay) {
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
            result = drs.returnItems(cID, itemID, numItems);

            printWithID(result.getData());
            printWithID("Finished client work");

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
		ClientReturnItems c = new ClientReturnItems("V43921", 2000, 5, 0);
		c.start();
	}
}