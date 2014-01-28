package corba.base;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.UnknownHostException;

/**
 * Simple wrapper, this class holds a server name string and a socketAddress to reach it.
 * It has methods to compress the representation using toString method.
 * It will parse a rawString via a similar method. This class should work seamlessly with UDPMessage.
 */
public class ServerAddress {
	private String name;
	private SocketAddress address;

	public ServerAddress(String name, SocketAddress addr) {
		this.name = name;
		this.address = addr;
	}

	public ServerAddress(String name, InetAddress address, int port) {
		this.name = name;
		this.address = new InetSocketAddress(address, port);
	}

	public static ServerAddress parseString(String rawString) throws UnknownHostException {
		String[] parsed = rawString.split(Message.SUB_VAL_SEP);

		String name = parsed[0];
		String address = parsed[1];
		int port = (int) Double.parseDouble(parsed[2]);
		InetAddress iAddr = InetAddress.getByName(address);

		ServerAddress sa =  new ServerAddress(name, iAddr, port);

		return sa;
	}

	public String getName() {
		return name;
	}

	public SocketAddress getAddress() {
		return address;
	}

	public int hashCode() {
		return name.hashCode();
	}

	public boolean equals(Object o) {
		if (this == o)
			return true;

		if (o == null)
			return false;

		ServerAddress other = (ServerAddress) o;
		return other.name.equals(name);
	}

	public String toString() {
		String str = "";
		String[] addressArray = address.toString().split(":");
		String addr = addressArray[0].substring(1);
		String port = addressArray[1];

		String[] array = {name, addr, port};

		for (String val : array)
			str += val + Message.SUB_VAL_SEP;

		return str;
	}

	public static void main(String[] args) throws UnknownHostException {
		InetAddress a = InetAddress.getByName("1.2.3.4");
		ServerAddress sa = new ServerAddress("V", a, 4444);
		String test = sa.toString();

		ServerAddress sa2 = ServerAddress.parseString(test);
		System.out.println(sa2);
	}
}
