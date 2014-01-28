package corba.server;

import java.io.File;
import java.util.Properties;
import java.util.Scanner;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;

import CORBAServer.RetailStore;
import CORBAServer.RetailStoreHelper;

import corba.base.DRServer;
import corba.server.CentralServer;
import corba.server.RetailStoreServerImpl;
import corba.server.RetailStoreUDP;

/**
 * Top level wrapper to start all the CORBA threads.
 */
public class CorbaStarter {
	public static void main(String[] args) {
		String[] servers = {"M", "T", "V"};

		CentralServer central = new CentralServer();
		central.start();

		try {
			// Parse args from config file if not on args.
			Scanner scan = new Scanner(new File(DRServer.ORBD_CONFIG));
			scan.nextLine();
			String argsLine = scan.nextLine();
			String[] orbArgs = argsLine.split(" ");

			// Orb properties to pass at init.
			Properties orbProps = System.getProperties();
			orbProps.setProperty("org.omg.CORBA.ORBClass", "com.sun.corba.se.internal.POA.POAORB");
			orbProps.setProperty("org.omg.CORBA.ORBSingletonClass", "com.sun.corba.se.internal.corba.ORBSingleton");

			// Init the ORB with the args passed.
			ORB orb = ORB.init(orbArgs, orbProps);

			// Get the root POA.
			POA poaRoot = (POA) orb.resolve_initial_references("RootPOA");
			poaRoot.the_POAManager().activate();

			// Get naming context for binding.
			org.omg.CORBA.Object objRef  = orb.resolve_initial_references("NameService");
			NamingContextExt nc = NamingContextExtHelper.narrow(objRef);

			/* For each server, bind the necessary associations in the nc. */
        	for (String sName : servers) {
        		// Create server and make interface reference.
        		RetailStoreServerImpl server = new RetailStoreServerImpl(sName);
        		server.setOrb(orb);
    			objRef = poaRoot.servant_to_reference(server);
    			RetailStore servRef = RetailStoreHelper.narrow(objRef);

  				NameComponent path[] = nc.to_name(sName);
				nc.rebind(path, servRef);

	        	RetailStoreUDP udp = new RetailStoreUDP(server);
	        	udp.start();
        	}

			System.out.println("CORBA Server ready...");

			// Wait for incoming requests
			orb.run();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
}