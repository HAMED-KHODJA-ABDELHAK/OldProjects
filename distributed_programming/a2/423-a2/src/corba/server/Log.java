package corba.server;

import java.io.File;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DateFormat;

import corba.base.DRServer;

/**
 * Generic logging service, mainly used for Servers to log information on transactions.
 *
 */
public class Log {
	private static final DateFormat dateFormat = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM);;
	private final static String BUY = DRServer.BUY_ITEM;
	private final static String RETURN = DRServer.RETURN_ITEM;
	private final static String EXCHANGE = DRServer.EXCHANGE;
	private final static String ERROR = "exception";

	private DataOutputStream log;
	private String cID;
	private String store;

	public Log(String store, String cID) {
		this.cID = cID;
		this.store = store;
	}

	public void openLog() throws FileNotFoundException {
		File directory = new File(store);
		if (!directory.exists())
			directory.mkdir();

		File file = new File(store + File.separator + cID);
		if (file.exists())
			this.log = new DataOutputStream(new FileOutputStream(file, true));
		else
			this.log = new DataOutputStream(new FileOutputStream(file));
	}

	private String currentDateTime() {
		return dateFormat.format(System.currentTimeMillis());
	}

	public void buyItems(int itemID, int number) throws IOException {
		log.writeBytes(String.format("(%s) %s %s %s: Purchased %d of %d.\n",
				currentDateTime(), BUY, store, cID, number, itemID));
		log.flush();
	}

	public void returnItems(int itemID, int number) throws IOException {
		log.writeBytes(String.format("(%s) %s %s %s: Returned %d of %d.\n",
				currentDateTime(), RETURN, store, cID, number, itemID));
		log.flush();
	}

	public void exchangeItems(int bItemID, int bNumItems, int dItemID, int dNumItems) throws IOException {
		log.writeBytes(String.format("(%s) %s %s %s: Returned %d of %d for %d of %d.\n",
				currentDateTime(), EXCHANGE, store, cID, bNumItems, bItemID, dNumItems, dItemID));
		log.flush();
	}

	public void text(String error) throws IOException {
		log.writeBytes(String.format("(%s) %s %s %s: %s\n",
				currentDateTime(), ERROR, store, cID, error));
		log.flush();
	}

	public void logClose() throws IOException {
		log.close();
	}

	public static void main(String[] args) {
		try {
			Log log = new Log("Montreal", "M49381");
			log.buyItems(4921, 2);
			log.returnItems(6210, 5);
			log.exchangeItems(2000, 5, 3000, 40);
			log.text("Failed to register with the central repository.");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
