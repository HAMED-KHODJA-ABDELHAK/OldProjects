package corba.base;

/**
 * Legacy file, used to hold RMI interface.
 * Now, holds text strings mainly for orbConfig.txt
 */
public interface DRServer {
    public static final String BUY_ITEM = "buyItems";
    public static final String RETURN_ITEM = "returnItems";
    public static final String CHECK_STOCK = "checkStock";
    public static final String EXCHANGE = "exchange";
    public static final String ORBD_CONFIG = "orbConfig.txt";
}
