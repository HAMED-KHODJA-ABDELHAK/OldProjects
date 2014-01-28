package corba.client;

import java.io.File;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.util.Properties;
import java.util.Scanner;

import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;

import corba.base.DRServer;
import corba.base.SendMessage;
import corba.base.Message;

public abstract class ClientBase implements Runnable {
	private int delay;
	private Thread thread;

	protected String cID;
	protected CORBAServer.RetailStore target = null;
	protected final int MAX_RETRIES = 4;

	public ClientBase(String cID, int delay) {
		this.cID = cID;
		this.delay = delay;
		thread = new Thread(this, cID);
	}

	public char determineServerName() {
		return cID.toUpperCase().charAt(0);
	}

	/** Ask the central for the server to contact.
	 */
	public String getClosestServer() throws IOException {
		/* Connect a new UDP socket, create a message and sockAddress. */
		DatagramSocket socket = new DatagramSocket();
		Message message = Message.nearestServerRequest(cID);
		InetAddress iAddr = InetAddress.getLocalHost();
		SocketAddress sAddress = new InetSocketAddress(iAddr, Message.getStartingPort());

		SendMessage.toServer(socket, message, sAddress);

		while (true) {
			/* Get UDP packet and parse it. */
			byte[] buffer = new byte[Message.getMaxBytes()];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			socket.receive(packet);
			message = new Message(packet);
			message.parseBytes();

			String cmd = message.getCmd();

			if (cmd.equals(Message.CMD_NEAR_SERVER_REPLY))
				return message.getArg(Message.STR_SERVER_NAME);
		}
	}

	public void start() {
		thread.start();
	}

	public void printWithID(String s) {
		System.out.println(cID + ":" + s);
	}

	@Override
	public void run() {
		try {
			Thread.sleep(delay);
			procedure();
			return;
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public void initORB() throws Exception {
		// Parse args from config file if not on args.
		Scanner scan = new Scanner(new File(DRServer.ORBD_CONFIG));
		scan.nextLine();
		String argsLine = scan.nextLine();
		String[] orbArgs = argsLine.split(" ");

		// Setup properties for ORB.
		Properties orbProps = System.getProperties();
		orbProps.setProperty("org.omg.CORBA.ORBClass", "com.sun.corba.se.internal.POA.POAORB");
		orbProps.setProperty("org.omg.CORBA.ORBSingletonClass", "com.sun.corba.se.internal.corba.ORBSingleton");

		// Initialize the ORB
		org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(orbArgs, orbProps);

		// Get the naming service.
		org.omg.CORBA.Object ncObj = orb.resolve_initial_references("NameService");
		NamingContextExt nc = NamingContextExtHelper.narrow(ncObj);

		// Resolve the method required to a stub reference.
		target = CORBAServer.RetailStoreHelper.narrow(nc.resolve_str(""+determineServerName()));
	}

	public abstract void procedure();
}