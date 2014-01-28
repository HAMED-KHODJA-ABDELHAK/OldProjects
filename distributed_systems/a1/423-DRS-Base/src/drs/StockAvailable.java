package drs;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class StockAvailable implements Serializable {
	private static final long serialVersionUID = 1211L;
	private HashMap<Character, Integer> storeStock;
	private int itemID;

	public StockAvailable(int itemID) {
		storeStock = new HashMap<Character, Integer>();
		this.itemID = itemID;
	}

	public void addStore(Character storeName, int numAvailable) {
		storeStock.put(storeName, numAvailable);
	}

	public int getItemID() {
		return itemID;
	}

	public Map<Character, Integer> getStoreMap() {
		return storeStock;
	}
}
