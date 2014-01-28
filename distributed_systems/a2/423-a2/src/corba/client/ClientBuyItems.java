package corba.client;

import CORBAServer.CorbaIOException;
import CORBAServer.CorbaInterruptedException;
import CORBAServer.Result_s;

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
			initORB();
			String store = cID.substring(0, 1);
            Result_s result = target.buyItems(cID, itemID, numItems, store);
            int retries = 1;

            while (!result.success && retries < MAX_RETRIES) {
            	if (!result.success)
                   	printWithID(result.message);

            	long delay =  (long) (1000  * Math.pow(2, retries) * Math.random());
            	printWithID(String.format("Failed on attempt %d, trying again after %dms.", retries, delay));
            	Thread.sleep(delay);

                result = target.buyItems(cID, itemID, numItems, store);

                ++retries;
            }

            if (result.success) {
	            printWithID(result.message);
	            printWithID("Finished client work");
            } else {
            	printWithID("Failed operation and reached max retries. Please try again later.");
            }

		} catch (InterruptedException e) {
			printWithID("Interrupted during sleep: " + e.getMessage());
		} catch (CorbaIOException e) {
			printWithID("Corba IOException: " + e.message);
		} catch (CorbaInterruptedException e) {
			printWithID("Corba InterruptedException" + e.message);
		} catch (Exception e) {
			printWithID("ORB Init Failed:" + e.getMessage());
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
