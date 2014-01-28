package corba.base;

import java.io.IOException;
import java.net.DatagramPacket;
import java.util.Collection;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * Class to marshal and unmarshal UDP messages.
 * Contains methods to query for args and create messages.
 * Defines base starting port for all UDP communication and max bytes transmitted.
 */
public class Message {
	private final static int STARTING_PORT = 20000;
	private final static int MAX_BYTES = 1400;
	private static long nextTID = 0;

	private final static String CMD_SEP = ":";
	private final static String ARG_SEP = ",";
	private final static String VAL_SEP = "-";
	private final static String END_MESSAGE = ")";
	public final static String SUB_VAL_SEP = "_";

	/* Used UDP commands follow: */
	public final static String CMD_JOIN_POOL_REQ = "joinPoolReq";
	public final static String CMD_JOIN_POOL_REPLY = "joinPoolReply";
	public final static String CMD_GET_STOCK_REQ = "getStockReq";
	public final static String CMD_GET_STOCK_REPLY = "getStockReply";
	public final static String CMD_NUM_SOLD_REQ = "numSoldReq";
	public final static String CMD_NUM_SOLD_REPLY = "numSoldReply";
	public final static String CMD_NEAR_SERVER_REQ = "nearServerReq";
	public final static String CMD_NEAR_SERVER_REPLY = "nearServerReply";

	/* UDP Strings used as argType follow: */
	public final static String INT_TID = "tID";
	public final static String INT_ITEM_ID = "itemID";
	public final static String INT_ITEM_STOCK = "itemStock";
	public final static String INT_NUM_SOLD = "numSold";
	public final static String INT_SYS_MILIS = "sysMilis";
	public final static String STR_CUST_ID = "cID";
	public final static String STR_SERVER_ADDR = "sAddr";
	public final static String STR_SERVER_NAME = "sName";

	private String message;
	private String cmd;
	private HashMap<String, ArrayList<String>> args;

	public Message(DatagramPacket packet) {
		message = new String(packet.getData());
		args = new HashMap<String, ArrayList<String>>();
	}

	private Message(String cmd) {
		this.cmd = cmd;
		this.args = new HashMap<String, ArrayList<String>>();

		addArg(INT_TID, getNextTID());
		addArg(INT_SYS_MILIS, System.currentTimeMillis());
	}

	/* Series of factory-ish methods. */
	public static Message joinPoolRequest(ServerAddress addr) {
		Message m = new Message(CMD_JOIN_POOL_REQ);

		m.addArg(STR_SERVER_ADDR, addr.toString());
		m.makeMessage();

		return m;
	}

	public static Message joinPoolReply(Collection<ServerAddress> addrList) {
		Message m = new Message(CMD_JOIN_POOL_REPLY);

		m.addServerAddrs(addrList);
		m.makeMessage();

		return m;
	}

	public static Message stockRequest(int itemID) {
		Message m = new Message(CMD_GET_STOCK_REQ);

		m.addArg(INT_ITEM_ID, itemID);
		m.makeMessage();

		return m;
	}

	public static Message stockReply(int itemID, int stock, String storeName) {
		Message m = new Message(CMD_GET_STOCK_REPLY);

		m.addArg(INT_ITEM_ID, itemID);
		m.addArg(INT_ITEM_STOCK, stock);
		m.addArg(STR_SERVER_NAME, storeName);
		m.makeMessage();

		return m;
	}

	public static Message numSoldRequest(int itemID, String cID) {
		Message m = new Message(CMD_NUM_SOLD_REQ);

		m.addArg(INT_ITEM_ID, itemID);
		m.addArg(STR_CUST_ID, cID);
		m.makeMessage();

		return m;
	}

	public static Message numSoldReply(int itemID, int numSold, String cID) {
		Message m = new Message(CMD_NUM_SOLD_REPLY);

		m.addArg(INT_ITEM_ID, itemID);
		m.addArg(INT_NUM_SOLD, numSold);
		m.addArg(STR_CUST_ID, cID);
		m.makeMessage();

		return m;
	}

	public static Message nearestServerRequest(String cID) {
		Message m = new Message(CMD_NEAR_SERVER_REQ);

		m.addArg(STR_CUST_ID, cID);
		m.makeMessage();

		return m;
	}

	public static Message nearestServerReply(String cID, String serverName) {
		Message m = new Message(CMD_NEAR_SERVER_REPLY);

		m.addArg(STR_SERVER_NAME, serverName);
		m.addArg(STR_CUST_ID, cID);
		m.makeMessage();

		return m;
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

	/**
	 * Reverse the process used in makeMessage to get all the information extracted.
	 */
	public void parseBytes() throws IOException {
		String[] splitCmd = message.split(CMD_SEP);

		cmd = splitCmd[0];
		String rest = splitCmd[1];

		rest = rest.substring(0, rest.lastIndexOf(Message.END_MESSAGE));
		String[] splitArgs = rest.split(ARG_SEP);

		for (String arg : splitArgs) {
			String[] splitVal = arg.split(VAL_SEP);
			String argName = splitVal[0], argVal = splitVal[1];
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

	public static int getStartingPort() {
		return STARTING_PORT;
	}

	public static int getMaxBytes() {
		return MAX_BYTES;
	}

	public synchronized static long getNextTID() {
		return nextTID++;
	}

	/**
	 * Use to add a set of values with a given argType, like serverList.
	 * If argType already in map, will overwrite.
	 */
	@SuppressWarnings("unused")
	private void addArgCollection(String argType, Collection<String> argCollection) {
		ArrayList<String> argList = new ArrayList<String>();

		argList.addAll(argCollection);

		args.put(argType, argList);
	}

	private void addServerAddrs(Collection<ServerAddress> servers) {
		ArrayList<String> argList = new ArrayList<String>();

		for (ServerAddress addr : servers) {
			argList.add(addr.toString());
		}

		args.put(STR_SERVER_ADDR, argList);
	}

	/**
	 * Add a single string value to arg map.
	 */
	private void addArg(String argType, String argVal) {
		ArrayList<String> argList = new ArrayList<String>();

		argList.add(argVal);

		args.put(argType, argList);
	}

	private void addArg(String argType, long intVal) {
		String strVal = ""+intVal;
		addArg(argType, strVal);
	}

	/**
	 * Flatten the command and args into a single String message.
	 */
	private void makeMessage() {
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
}
