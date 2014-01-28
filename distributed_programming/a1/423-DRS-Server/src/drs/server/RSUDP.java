package drs.server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.ArrayList;

import drs.UDPMessage;


/**
 * Acts as a communication front to the retail store.
 *
 */
public class RSUDP implements Runnable {
	private RS retail;
	private Thread thread;
	private DatagramSocket socket;
	private boolean running;

	public RSUDP(RS retailStore) {
		retail = retailStore;
		thread = new Thread(this);
		thread.setName(RSUDP.class.toString() + "-" + retail.getStoreName());
		running = false;
	}

	public void start() {
		running = true;
		thread.start();
	}

	public void stop() {
		running = false;
	}

	private void updateServerList(ArrayList<String> list) {
		for (String s : list) {
			retail.addServer(s.toUpperCase().charAt(0));
		}
	}

	private void sendStockInfo(String itemStr, InetAddress addr, int port) throws IOException {
		int itemID = 0, stock = 0;
		itemID = (int) Double.parseDouble(itemStr); // Odd bug with Integer.parseInt, using double parsing instead.
		Item item = retail.getItem(itemID);

		if (item != null)
			stock = item.getStock();

		UDPMessage reply = new UDPMessage(UDPMessage.CMD_STOCK_INFO);
		reply.addArg(UDPMessage.ARG_ITEM_ID, ""+itemID);
		reply.addArg(UDPMessage.ARG_ITEM_STOCK, ""+stock);
		reply.addArg(UDPMessage.ARG_ITEM_STORE, ""+retail.getStoreName());
		reply.makeMessage();

		byte[] buf = reply.getBytes();

		DatagramPacket info = new DatagramPacket(buf, buf.length, addr, port);
		socket.send(info);
	}

	@Override
	public void run() {
		try {
			socket = new DatagramSocket(UDPMessage.STARTING_PORT + retail.getStoreName());

			UDPMessage message = new UDPMessage(UDPMessage.CMD_JOIN_SERVER_POOL);
			message.addArg(UDPMessage.ARG_SERVER_NAME, ""+retail.getStoreName());
			message.makeMessage();
			byte[] buf = message.getBytes();

			DatagramPacket request = new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(),
					UDPMessage.STARTING_PORT);
			socket.send(request);

			while (running) {
				buf = new byte[UDPMessage.MAX_BYTES];
				DatagramPacket reply = new DatagramPacket(buf, buf.length);

				socket.receive(reply);

				InetAddress addr = reply.getAddress();
				int port = reply.getPort();

				message = new UDPMessage(reply.getData());
				message.parseBytes();
				String cmd = message.getCmd();

				if (cmd.equals(UDPMessage.CMD_SERVER_LIST)) {
					ArrayList<String> list = message.getArgList(UDPMessage.ARG_SERVER_NAME);
					updateServerList(list);
				} else if (cmd.equals(UDPMessage.CMD_GET_STOCK)) {
					String arg = message.getArg(UDPMessage.ARG_ITEM_ID);
					sendStockInfo(arg, addr, port);
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
