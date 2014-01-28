package corba.client;

import CORBAServer.CorbaIOException;
import CORBAServer.CorbaNotEnoughStockException;
import CORBAServer.Result_s;

public class ClientExchangeItems extends ClientBase {
	private int bItemID;
	private int bNumItems;
	private int dItemID;
	private int dNumItems;

	public ClientExchangeItems(String cID, int bItemID, int bNumItems,
			int dItemID, int dNumItems, int delay) {
		super(cID, delay);

		this.bItemID = bItemID;
		this.bNumItems = bNumItems;
		this.dItemID = dItemID;
		this.dNumItems = dNumItems;
	}

	@Override
	public void procedure() {
		try {
			initORB();
			Result_s result = target.exchange(cID, bItemID, bNumItems, dItemID, dNumItems);
            int retries = 1;

            while (!result.success && retries < MAX_RETRIES) {
            	if (!result.success)
                   	printWithID(result.message);

            	long delay =  (long) (1000  * Math.pow(2, retries) * Math.random());
            	printWithID(String.format("Failed on attempt %d, trying again after %dms.", retries, delay));
            	Thread.sleep(delay);

                result = target.exchange(cID, bItemID, bNumItems, dItemID, dNumItems);

                retries++;
            }

            if (result.success) {
	            printWithID(result.message);
	            printWithID("Finished client work");
            } else {
            	printWithID("Failed operation and reached max retries. Please try again later.");
            	printWithID(result.message);
            }

		} catch (CorbaNotEnoughStockException e) {
			printWithID("NotEnoughStock: " + e.message);
		} catch (CorbaIOException e) {
			printWithID("CORBA IO:" + e.message);
		} catch (Exception e) {
			printWithID("ORB Init Failed:" + e.getMessage());
		}
	}

	/* Created a series of logs just for this.
	 * User V45000 has made following transactions:
	 */
	public static void main(String[] args) {
		/* Didn't purchase enough from stores, customer only purchased 1 of 3000. */
		ClientExchangeItems c1 = new ClientExchangeItems("V45000", 3000, 2, 2500, 6, 0);
		c1.start();

		/* Not enough stock in stores, client bought 4 of 3000 from T. */
		ClientExchangeItems c2 = new ClientExchangeItems("V45001", 3000, 2, 2500, 100, 0);
		c2.start();

		/* Works, enough stock and purchases. User purchased 10 accross all stores, inventory has six at first store. */
		ClientExchangeItems c3 = new ClientExchangeItems("V45002", 2000, 8, 2500, 6, 0);
		c3.start();
	}
}
