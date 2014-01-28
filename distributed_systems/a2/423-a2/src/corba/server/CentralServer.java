package corba.server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;

import corba.base.SendMessage;
import corba.base.ServerAddress;
import corba.base.Message;

/**
 * Defaults to using port 20000 for communication, can be overridden at construction time.
 * Acts as a central hub to make sure all servers have an up to date list of all other servers.
 * On request of a join, will add to the current list and then send out the updated list to all servers.
 * A short time after all servers created, should know all other servers.
 */
public class CentralServer implements Runnable {
	private int serverPort = Message.getStartingPort();
	private HashMap<String, ServerAddress> addrDB;
	private DatagramSocket socket;
	private Thread thread;
	private boolean running;

	public CentralServer() {
		addrDB = new HashMap<String, ServerAddress>();
		running = false;
		thread = new Thread(this, CentralServer.class.toString());
	}

	public CentralServer(int port) {
		serverPort = port;
		addrDB = new HashMap<String, ServerAddress>();
		running = false;
		thread = new Thread(this, CentralServer.class.toString());
	}

	public void start() {
		running = true;
		thread.start();
	}

	public void stop() {
		running = false;
	}

	/**
	 * Start the server on the designated port.
	 * Upon receipt of a join message, advise all servers
	 */
	@Override
	public void run() {
		try {
			socket = new DatagramSocket(serverPort);

			while (running) {
				byte[] buffer = new byte[Message.getMaxBytes()];

				/* Get UDP packet and parse it. */
				DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
				socket.receive(packet);
				Message message = new Message(packet);
				message.parseBytes();

				String cmd = message.getCmd();

				/* Process commands here. */
				if (cmd.equals(Message.CMD_JOIN_POOL_REQ)) {
					String arg = message.getArg(Message.STR_SERVER_ADDR);
					updateServerPool(arg);
				} else if (cmd.equals(Message.CMD_NEAR_SERVER_REQ)) {
					int port = packet.getPort();
					InetAddress addr = packet.getAddress();

					String cID = message.getArg(Message.STR_CUST_ID);
					sendNearestServer(cID, addr, port);
				}
			}
		} catch (IOException e) {
			System.out.println("IO: " + e.getMessage());
			if (socket != null)
				socket.close();
		}

		return;
	}

	/**
	 * Function determines the closest server to a given client based on preferred in cID.
	 * If the client's preferred store exists, send that address.
	 * If it doesn't, find the next store that follows the preferred.
	 */
	private String getNearestAddress(String cID) {
		String prefStore = cID.substring(0, 1);
		String targetStore = null;

		if (addrDB.keySet().contains(prefStore)) {
			targetStore = addrDB.get(prefStore).getName();
		} else {
			LinkedList<String> keys = new LinkedList<String>(addrDB.keySet());
			Collections.sort(keys);

			for (String s : keys) {
				if (prefStore.compareTo(s) < 0) {
					targetStore = addrDB.get(s).getName();
					break;
				}
			}
		}

		return targetStore;
	}

	/**
	 * Send the nearest store back to the client. Reply back to whatever addr/port request originates from.
	 */
	private void sendNearestServer(String cID, InetAddress addr, int port) throws IOException {
		String nearest = getNearestAddress(cID);

		Message message = Message.nearestServerReply(cID, nearest);
		SendMessage.toServer(socket, message, new InetSocketAddress(addr, port));
	}

	/**
	 * Add the server to pool, send the new complete list to all other servers.
	 */
	private void updateServerPool(String rawString) throws IOException {
		ServerAddress sa = ServerAddress.parseString(rawString);

		addrDB.put(sa.getName(), sa);

		/* Create message with list of servers */
		Message message = Message.joinPoolReply(addrDB.values());

		/* Send to each member. */
		SendMessage.toAllServers(socket, message, addrDB.values());
	}

	public static void main(String[] args) {
		CentralServer s = new CentralServer();
		System.out.println("Starting server.");
		s.start();
	}
}