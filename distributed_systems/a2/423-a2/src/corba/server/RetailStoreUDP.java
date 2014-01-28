package corba.server;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;

import corba.base.SendMessage;
import corba.base.ServerAddress;
import corba.base.Message;


/**
 * Acts as a communication front to the retail store.
 *
 */
public class RetailStoreUDP implements Runnable {
	private RetailStoreServerImpl retail;
	private Thread thread;
	private DatagramSocket socket;
	private boolean running;

	public RetailStoreUDP(RetailStoreServerImpl retailStore) {
		retail = retailStore;
		thread = new Thread(this);
		thread.setName(RetailStoreUDP.class.toString() + "-" + retail.getStoreName());
		running = false;
	}

	public void start() {
		running = true;
		thread.start();
	}

	public void stop() {
		running = false;
	}

	private void updateServerList(ArrayList<String> list) throws UnknownHostException {
		ServerAddress sa;

		for (String s : list) {
			sa = ServerAddress.parseString(s);
			retail.addServer(sa);
		}
	}

	private void sendStockInfo(String itemStr, InetAddress addr, int port) throws IOException {
		int itemID = 0, stock = 0;
		itemID = (int) Double.parseDouble(itemStr); // Odd bug with Integer.parseInt, using double parsing instead.
		Item item = retail.getItem(itemID);

		if (item != null)
			stock = item.getStock();

		Message message = Message.stockReply(itemID, stock, retail.getStoreName());
		SendMessage.toServer(socket, message, new InetSocketAddress(addr, port));
	}

	private void sendLogSales(String cID, int itemID, InetAddress addr, int port) throws IOException {
		int numSold = 0;

		try {
			LogParser l = new LogParser("" +retail.getStoreName(), cID, itemID);
			l.openLog();
			numSold = l.getNumItemsInLog();
		} catch (FileNotFoundException e) {
			// Ignore, means no log was found.
		}

		Message message = Message.numSoldReply(itemID, numSold, cID);
		SendMessage.toServer(socket, message, new InetSocketAddress(addr, port));
	}


	@Override
	public void run() {
		try {
			socket = new DatagramSocket(0, InetAddress.getLocalHost());

			// Create this servers entry.
			ServerAddress sa = new ServerAddress(retail.getStoreName(), socket.getLocalAddress(), socket.getLocalPort());
			System.out.println("Starting a UDP Thread: " + sa.toString());

			Message message = Message.joinPoolRequest(sa);
			SendMessage.toServer(socket, message, new InetSocketAddress(InetAddress.getLocalHost(),
					Message.getStartingPort()));

			while (running) {
				byte[] buf = new byte[Message.getMaxBytes()];
				DatagramPacket reply = new DatagramPacket(buf, buf.length);

				socket.receive(reply);

				InetAddress replyAddr = reply.getAddress();
				int replyPort = reply.getPort();

				message = new Message(reply);
				message.parseBytes();
				String cmd = message.getCmd();

				if (cmd.equals(Message.CMD_JOIN_POOL_REPLY)) {
					ArrayList<String> list = message.getArgList(Message.STR_SERVER_ADDR);
					updateServerList(list);
				} else if (cmd.equals(Message.CMD_GET_STOCK_REQ)) {
					String arg = message.getArg(Message.INT_ITEM_ID);
					sendStockInfo(arg, replyAddr, replyPort);
				} else if (cmd.equals(Message.CMD_NUM_SOLD_REQ)) {
					String cID = message.getArg(Message.STR_CUST_ID);
					int itemID = (int) Double.parseDouble(message.getArg(Message.INT_ITEM_ID));
					sendLogSales(cID, itemID, replyAddr, replyPort);
				}
			}
		} catch (SocketException e) {
			System.out.println("Socket: " + e.getMessage());
		} catch (IOException e) {
			System.out.println("IO: " + e.getMessage());
		} finally {
			if (socket != null)
				socket.close();
		}
	}
}
