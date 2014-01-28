package drs;

import java.io.IOException;
import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface DRServer extends Remote {
    public static final String BUY_ITEM = "buyItems";
    public static final String RETURN_ITEM = "returnItems";
    public static final String CHECK_STOCK = "checkStock";

    public TransactionResult buyItems(String cID, int itemID, int numItems)
    		throws RemoteException, IOException, NotBoundException, InterruptedException;

    public TransactionResult returnItems(String cID, int itemID, int numItems)
    		throws RemoteException, IOException, InterruptedException;

    public StockAvailable checkStock(int itemID)
    		throws RemoteException, IOException;
}
