package drs.server;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;

import drs.DRServer;
import drs.RmiStarter;

public class ServerStarter extends RmiStarter {
	public ServerStarter() {
		super(DRServer.class);
	}

	@Override
	public void startupProcedure() {
		ArrayList<Character> servers = new ArrayList<Character>();
		servers.add('M');
		servers.add('V');
		servers.add('T');

		/* Start the central server. */
		CentralServer central = new CentralServer();
		central.start();

		/* Start the retail servers from a given list. Default inventories generated. */
        try {
        	for (char c : servers) {
        		RS server = new RS(c);
	        	DRServer drs = (DRServer) server;
	        	DRServer drsStub = (DRServer)UnicastRemoteObject.exportObject(drs, 0);
	        	RSUDP udp = new RSUDP(server);
	        	udp.start();

	            Registry registry = LocateRegistry.getRegistry();
	            registry.rebind(c + DRServer.BUY_ITEM, drsStub);
	            registry.rebind(c + DRServer.RETURN_ITEM, drsStub);
	            registry.rebind(c + DRServer.CHECK_STOCK, drsStub);
        	}
        }
        catch(Exception e) {
            e.printStackTrace();
        }
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new ServerStarter();

		while (true);
	}
}
