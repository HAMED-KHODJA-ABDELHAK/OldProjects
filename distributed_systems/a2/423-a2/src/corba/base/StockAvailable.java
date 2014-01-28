package corba.base;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

import CORBAServer.Stock_s;

public class StockAvailable implements Serializable {
	private static final long serialVersionUID = 1211L;
	private HashMap<String, Integer> storeStock;
	private int itemID;

	public StockAvailable(int itemID) {
		storeStock = new HashMap<String, Integer>();
		this.itemID = itemID;
	}

	public void addStore(String storeName, int numAvailable) {
		storeStock.put(storeName, numAvailable);
	}

	public int getItemID() {
		return itemID;
	}

	public Map<String, Integer> getStoreMap() {
		return storeStock;
	}

	/* Flatten the standard map down to the corba array. */
	public Stock_s[] getArray() {
		Stock_s[] array = new Stock_s[storeStock.size()];
		int i = 0;

		for (String s : storeStock.keySet())
			array[i++] = new Stock_s(s, storeStock.get(s));

		return array;
	}
}
