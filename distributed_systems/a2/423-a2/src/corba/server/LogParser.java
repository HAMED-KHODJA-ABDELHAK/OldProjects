package corba.server;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.StringTokenizer;

import corba.base.DRServer;

/*
 * Simple log parser, it exists only to analyse how much a client has bought from a store, minus returns.]
 * This positive number represents the max the customer can return to the store.
 *
 * Format of log:
 *
 * buyItems:
 * (29-Oct-2012 4:06:01 PM) buyItems M M43932: Purchased 4 of 2000.
 *
 * returnItems:
 * (29-Oct-2012 4:06:01 PM) returnItems V M43932: Returned 3 of 2000. */
public class LogParser {
	private Scanner log;
	private String cID;
	private String store;
	private int itemID;

	@SuppressWarnings("unused")
	private LogParser() {};

	public LogParser (String store, String cID, int itemID) {
		this.cID = cID;
		this.store = store;
		this.itemID = itemID;
	}

	public void openLog() throws FileNotFoundException {
		File file = new File(store + File.separator + cID);
		log = new Scanner(file);
	}

	private int parseNumSold(String line) {
		/* Long line, skip boring stuff. See format at top. */
		StringTokenizer tok = new StringTokenizer(line);
		int i = 0;
		while (i++ < 7)
			tok.nextToken();

		/* Parse all elements of importance. */
		int logNumSold = (int) Double.parseDouble(tok.nextToken());
		tok.nextToken();
		int logItemID = (int) Double.parseDouble(tok.nextToken());

		/* We only care if itemID matches. */
		if (itemID == logItemID)
			return logNumSold;
		else
			return 0;
	}

	private int parseNumReturned(String line) {
		/* Long line, skip boring stuff. See format at top. */
		StringTokenizer tok = new StringTokenizer(line);
		int i = 0;
		while (i++ < 7)
			tok.nextToken();

		/* Parse all elements of importance. */
		int logNumReturned = (int) Double.parseDouble(tok.nextToken());
		tok.nextToken();
		int logItemID = (int) Double.parseDouble(tok.nextToken());

		/* We only care if itemID matches. */
		if (itemID == logItemID)
			return logNumReturned;
		else
			return 0;

	}

	public int getNumItemsInLog() {
		int total = 0;

		while(log.hasNextLine()) {
			String line = log.nextLine();

			/* If the line not a sale or return, skip. */
			if (line.contains(DRServer.EXCHANGE))
				continue;
			else if (line.contains(DRServer.BUY_ITEM))
				total += parseNumSold(line);
			else if (line.contains(DRServer.RETURN_ITEM))
				total -= parseNumReturned(line);
		}

		return total;
	}

	public static void main(String[] args) throws FileNotFoundException {
		LogParser l = new LogParser("V", "M43932", 2000);
		l.openLog();
		System.out.println(l.getNumItemsInLog());
	}
}
