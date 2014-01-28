package drs.server;

import java.util.concurrent.Semaphore;


/**
 * Class assumptions:
 * itemID is a unique 4 digit number, won't repeat within a given store.
 * Item has a semaphore instead of synchronization to allow item locking during recursive RMI operation.
 * Semaphore should be taking before buy and return.
 *
 */
public class Item {
	private final int itemID;
	private int stock;
	private Semaphore sem;

	@SuppressWarnings("unused")
	private Item() {
		throw new Error();
	}

	public Item(int itemID, int stock) {
		this.itemID = itemID;
		this.stock = stock;
		this.sem = new Semaphore(1, true);
	}

	public Item(Item i) {
		this.itemID = i.itemID;
		this.stock = i.stock;
		this.sem = new Semaphore(1, true);
	}

	public synchronized boolean tryDownSem() {
		return sem.tryAcquire();
	}

	public synchronized void downSem() throws InterruptedException {
		sem.acquire();
	}

	public synchronized void upSem() {
		sem.release();
	}

	public void buyItems(int numSold) {
		if (numSold <= stock)
			stock -= numSold;
	}

	public void returnItems(int numReturned) {
		stock += numReturned;
	}

	public int getStock() {
		return this.stock;
	}

	public int getItemID() {
		return this.itemID;
	}

	@Override
    public int hashCode() {
		return itemID;
    }

	@Override
    public boolean equals(Object obj) {
    	if (this == obj)
    		return true;

    	if (obj == null || obj.getClass() != this.getClass())
    		return false;

    	Item item2 = (Item) obj;
    	return itemID == item2.itemID;
    }

	@Override
    public String toString() {
    	return String.format("%d available for item %d.\n", stock, itemID);
    }
}
