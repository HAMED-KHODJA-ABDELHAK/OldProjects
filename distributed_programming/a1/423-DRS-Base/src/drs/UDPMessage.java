package drs;

import java.io.IOException;
import java.util.Collection;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * Class to marshal and unmarshal UDP messages.
 * Contains methods to query for args and create messages.
 * Defines base starting port for all UDP communication and max bytes transmitted.
 */
public class UDPMessage {
	public final static int STARTING_PORT = 20000;
	public final static int MAX_BYTES = 1400;

	public final static String ID_SEP = "$";
	public final static String CMD_SEP = ":";
	public final static String ARG_SEP = ",";
	public final static String VAL_SEP = "-";
	public final static String END_MESSAGE = ".";

	/* Used UDP commands follow: */
	public final static String CMD_JOIN_SERVER_POOL = "joinPool"; /* Format: joinServerPool:servername-name\n */
	public final static String CMD_SERVER_LIST = "serverList"; /* Format: serverList:serverName-name,serverName-name,serverName-name... */
	public final static String CMD_GET_STOCK = "getStock";
	public final static String CMD_STOCK_INFO = "stockInfo";

	/* UDP Strings used as argType follow: */
	public final static String ARG_SERVER_NAME = "sName";
	public final static String ARG_ID = "id";
	public final static String ARG_ITEM_ID = "itemID";
	public final static String ARG_ITEM_STOCK = "itemStock";
	public final static String ARG_ITEM_STORE = "itemStore";

	private String message;
	private String cmd;
	private String id; /* Concatenate a string identifying origin with an ID #, like name-id */
	private HashMap<String, ArrayList<String>> args;

	public UDPMessage(byte[] b) {
		message = new String(b);
		args = new HashMap<String, ArrayList<String>>();
	}

	public UDPMessage(String cmd) {
		this.cmd = cmd;
		this.args = new HashMap<String, ArrayList<String>>();
	}

	public UDPMessage(String cmd, String origin, int transactNum) {
		this.cmd = cmd;
		this.id = origin + ID_SEP + transactNum;

		this.args = new HashMap<String, ArrayList<String>>();
		ArrayList<String> a = new ArrayList<String>();
		a.add(id);
		args.put(ARG_ID, a);
	}

	/**
	 * Use to add a set of values with a given argType, like serverList.
	 * If argType already in map, will overwrite.
	 */
	public void addArgCollection(String argType, Collection<String> argCollection) {
		ArrayList<String> argList = new ArrayList<String>();

		for (String s : argCollection) {
			argList.add(s);
		}

		args.put(argType, argList);
	}

	/**
	 * Add a single string value to arg map.
	 */
	public void addArg(String argType, String argVal) {
		ArrayList<String> argList = new ArrayList<String>();

		argList.add(argVal);

		args.put(argType, argList);
	}

	/**
	 * Flatten the command and args into a single String message.
	 */
	public void makeMessage() {
		message = cmd + CMD_SEP;

		for (String key : args.keySet()) {
			ArrayList<String> a = args.get(key);

			for (String val : a) {
				message += key + VAL_SEP + val + ARG_SEP;
			}
		}
		message = message.substring(0, message.length() -1);

		message += END_MESSAGE;
	}

	/**
	 * Reverse the process used in makeMessage to get all the information extracted.
	 * If any problems occur, throw an exception.
	 * @throws IOException
	 */
	public void parseBytes() throws IOException {
		String[] splitCmd = message.split(CMD_SEP);

		if (splitCmd.length != 2)
			throw new IOException();

		cmd = splitCmd[0];
		String rest = splitCmd[1];

		String[] splitArgs = rest.split(ARG_SEP);

		for (String arg : splitArgs) {
			String[] splitVal = arg.split(VAL_SEP);
			String argName = splitVal[0], argVal = splitVal[1];
			argVal = argVal.replace(".", "");
			ArrayList<String> list = args.get(argName);

			if (list == null)
				list = new ArrayList<String>();

			list.add(argVal);
			args.put(argName, list);
		}
	}

	/**
	 * Return the message in bytes, assumes only one packet ever needed.
	 * @param number
	 * @return
	 */
	public byte[] getBytes() {
		return message.getBytes();
	}

	/**
	 * Used to determine the num packets needed.
	 * @return
	 */
	public int getNumPackets() {
		return message.getBytes().length / MAX_BYTES + 1;
	}

	public String getCmd() {
		return cmd;
	}

	/* Returns an array list containing all values for argName. If none exist, returns null. */
	public String getArg(String argName) {
		ArrayList<String> list = args.get(argName);
		if (list != null)
			return list.get(0);
		else
			return null;
	}

	/* Returns an array list containing all values for argName. If none exist, returns null. */
	public ArrayList<String> getArgList(String argName) {
		return args.get(argName);
	}

	public static void main(String[] args) throws Exception {
		UDPMessage m = new UDPMessage(UDPMessage.CMD_JOIN_SERVER_POOL);
		ArrayList<String> a = new ArrayList<String>();
		a.add("Dog");
		a.add("Cat");
		a.add("Nothing");

		m.addArg(UDPMessage.ARG_SERVER_NAME, "D");
		m.addArgCollection("listThings", a);

		m.makeMessage();
		System.out.println(m.message);

		byte[] b = m.getBytes();
		System.out.println(b.length);

		UDPMessage m2 = new UDPMessage(b);
		m2.parseBytes();
		System.out.println(m2.getCmd());
		for (String s : m2.getArgList("listThings")) {
			System.out.println(s);
		}

		System.out.println(m2.getArg("test"));

	}
}
