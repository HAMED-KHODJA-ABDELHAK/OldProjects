package corba.client;

import java.io.IOException;

import CORBAServer.CorbaIOException;
import CORBAServer.Stock_s;

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
			initORB();
            Stock_s[] stock = target.checkStock(itemID);

            printWithID(String.format("For the item %d the following stores have:\n", itemID));

            for (Stock_s s : stock)
            	printWithID(String.format("The store %s had %d of the item %d.\n", s.storeName, s.num, itemID));

            printWithID("Finished client work");
		} catch (CorbaIOException e) {
			printWithID("CORBA IO:" + e.message);
		} catch (Exception e) {
			printWithID("ORB Init Failed:" + e.getMessage());
		}
	}

	public static void main(String[] args) throws IOException {
		ClientCheckStock c = new ClientCheckStock("M43921", 664, 0);
		c.start();

		ClientCheckStock c2 = new ClientCheckStock("V39211", 2000, 0);
		c2.start();
	}
}