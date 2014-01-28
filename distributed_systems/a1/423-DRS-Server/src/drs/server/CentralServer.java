package drs.server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.HashSet;

import drs.UDPMessage;

/**
 * Defaults to using port 20000 for communication, can be overridden at construction time.
 * Acts as a central hub to make sure all servers have an up to date list of all other servers.
 * On request of a join, will add to the current list and then send out the updated list to all servers.
 * A short time after all servers created, should know all other servers.
 */
public class CentralServer implements Runnable {
	private int serverPort = UDPMessage.STARTING_PORT;
	private HashSet<String> db;
	private DatagramSocket socket;
	private Thread thread;
	private boolean running;

	public CentralServer() {
		db = new HashSet<String>();
		thread = new Thread(this);
		running = false;
		thread.setName(CentralServer.class.toString());
	}

	public CentralServer(int port) {
		serverPort = port;
		db = new HashSet<String>();
		thread = new Thread(this);
		running = false;
		thread.setName(CentralServer.class.toString());
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
				byte[] buffer = new byte[UDPMessage.MAX_BYTES];

				/* Get UDP packet and parse it. */
				DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
				socket.receive(packet);
				UDPMessage message = new UDPMessage(packet.getData());
				message.parseBytes();

				String cmd = message.getCmd();

				/* Process commands here. */
				if (cmd.equals(UDPMessage.CMD_JOIN_SERVER_POOL)) {
					String arg = message.getArg(UDPMessage.ARG_SERVER_NAME);
					updateServerPool(arg);
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
	 * Add the server to pool, send the new complete list to all other servers.
	 */
	private void updateServerPool(String name) throws IOException {
		db.add(name.toUpperCase().substring(0, 1));

		/* Create message with list of servers */
		UDPMessage m = new UDPMessage(UDPMessage.CMD_SERVER_LIST);
		m.addArgCollection(UDPMessage.ARG_SERVER_NAME, db);
		m.makeMessage();
		byte[] buf = m.getBytes();

		/* Send to each member. */
		for (String s : db) {
			char server = s.charAt(0);

			DatagramPacket packet = new DatagramPacket(buf, buf.length,
					InetAddress.getLocalHost(), UDPMessage.STARTING_PORT + server);
			socket.send(packet);
		}
	}

	public static void main(String[] args) {
		CentralServer s = new CentralServer();
		System.out.println("Starting server.");
		s.start();
	}
}