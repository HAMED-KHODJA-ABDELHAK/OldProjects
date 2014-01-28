package drs.server;

import java.io.File;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DateFormat;

/**
 * Generic logging service, mainly used for Servers to log information on transactions.
 *
 */
public class Log {
	private static final DateFormat dateFormat = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM);;
	private final static String BUY = drs.DRServer.BUY_ITEM;
	private final static String RETURN = drs.DRServer.RETURN_ITEM;
	private final static String ERROR = "exception";

	private final DataOutputStream log;
	private final String cID;
	private final String store;

	@SuppressWarnings("unused")
	private Log() {
		throw new Error();
	}

	public Log(String store, String cID) throws FileNotFoundException  {
		this.cID = cID;
		this.store = store;

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
			log.text("Failed to register with the central repository.");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
