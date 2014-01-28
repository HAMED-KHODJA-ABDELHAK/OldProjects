package corba.base;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.util.Collection;

/**
 * Helper class, exists only to do perform sends for messages.
 */
public class SendMessage {
	public static void toServer(DatagramSocket socket, Message message, ServerAddress addr)
			throws IOException {
		byte[] buf = message.getBytes();

		DatagramPacket packet = new DatagramPacket(buf, buf.length, addr.getAddress());
		socket.send(packet);
	}

	public static void toServer(DatagramSocket socket, Message message, SocketAddress addr)
			throws IOException {
		byte[] buf = message.getBytes();

		DatagramPacket packet = new DatagramPacket(buf, buf.length, addr);
		socket.send(packet);
	}

	public static void toAllServers(DatagramSocket socket, Message message, Collection<ServerAddress> servers)
			throws IOException {
		byte[] buf = message.getBytes();

		for (ServerAddress addr : servers) {
			DatagramPacket packet = new DatagramPacket(buf, buf.length, addr.getAddress());
			socket.send(packet);
		}
	}
}
