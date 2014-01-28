package corba.client;

import CORBAServer.CorbaIOException;
import CORBAServer.CorbaInterruptedException;
import CORBAServer.Result_s;

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
			initORB();
            Result_s result = target.returnItems(cID, itemID, numItems);

            printWithID(result.message);
            printWithID("Finished client work");

		} catch (CorbaIOException e) {
			printWithID("Corba IOException: " + e.message);
		} catch (CorbaInterruptedException e) {
			printWithID("Corba InterruptedException:" + e.message);
		} catch (Exception e) {
			printWithID("ORB Init Failed:" + e.getMessage());
		}
	}

	public static void main(String[] args) {
		ClientReturnItems c = new ClientReturnItems("V43921", 2000, 5, 0);
		c.start();
	}
}