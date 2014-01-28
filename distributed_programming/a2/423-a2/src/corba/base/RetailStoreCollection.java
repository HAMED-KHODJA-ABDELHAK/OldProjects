package corba.base;
import java.util.Hashtable;


public interface RetailStoreCollection {

	public Boolean BuyItems(String customerID, String itemID, int numberOfItems);
	public Boolean ReturnItems(String customerID, String itemID, int numberOfItems);
	public Boolean ExchangeItems(String customerID, String itemID, int numberOfItems, String newItemID, int newNumberOfItems);
	public Hashtable<Character, Integer> CheckStockItems(String itemID);
}
