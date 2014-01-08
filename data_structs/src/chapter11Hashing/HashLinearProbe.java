package chapter11Hashing;

import java.util.Scanner;

class DataItem {
	private int iData;

	public DataItem(int i) {
		iData = i;
	}

	public int getKey() {
		return iData;
	}

	@Override
	public String toString() {
		return String.format("%d ", iData);
	}
}

class HashTable {
	private DataItem[] hashArray;
	private int arraySize;
	private DataItem deletedItem;

	public HashTable(int size) {
		arraySize = size;
		hashArray = new DataItem[arraySize];
		deletedItem = new DataItem(-1);
	}

	public void displayTable() {
		System.out.print("Table: ");

		for (DataItem d : hashArray) {
			if (d != null)
				System.out.print(d);
			else
				System.out.print("** ");
		}

		System.out.println();
	}

	public int hashFunc(int key) {
		return key % arraySize;
	}

	public void insert(DataItem item) {
		int hashVal = hashFunc(item.getKey());

		while (hashArray[hashVal] != null && hashArray[hashVal].getKey() != -1) {
			hashVal = (hashVal + 1) % arraySize;
		}

		hashArray[hashVal] = item;
	}

	public DataItem delete(int key) {
		int hashVal = hashFunc(key);

		while (hashArray[hashVal] != null) {
			if (hashArray[hashVal].getKey() == key) {
				DataItem temp = hashArray[hashVal];
				hashArray[hashVal] = deletedItem;
				return temp;
			}

			hashVal = (hashVal + 1) % arraySize;
		}

		return null;
	}

	public DataItem find(int key) {
		int hashVal = hashFunc(key);

		while (hashArray[hashVal] != null) {
			if (hashArray[hashVal].getKey() == key)
				return hashArray[hashVal];

			hashVal = (hashVal + 1) % arraySize;
		}

		return null;
	}
}

public class HashLinearProbe {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int KEYS_PER_CELL = 10;
		int size, n, key;
		Scanner s = new Scanner(System.in);

		System.out.println("Enter the size of the hash table: ");
		size = s.nextInt();
		System.out.println("Enter the initial number of items: ");
		n = s.nextInt();

		HashTable h = new HashTable(size);

		for (int i = 0; i < n; ++i) {
			key = (int) (Math.random() * KEYS_PER_CELL * size);
			h.insert(new DataItem(key));
		}

		while (true) {
			System.out.println("Select a command from the following: ");
			System.out.println("(d)isplay, (i)nsert, (r)emove, (f)ind.");

			char choice = s.next().toLowerCase().charAt(0);

			switch (choice) {
			case 'd':
				h.displayTable();
				break;

			case 'i':
				System.out.print("Enter key value to insert: ");
				key = s.nextInt();
				h.insert(new DataItem(key));
				break;

			case 'r':
				System.out.println("Enter key to remove: ");
				key = s.nextInt();
				h.delete(key);
				break;

			case 'f':
				System.out.println("Enter the key value to find: ");
				key = s.nextInt();
				DataItem d = h.find(key);

				if (d != null)
					System.out.printf("Found item with key %d.\n", key);
				else
					System.out.printf("Did not find key %d.\n", key);

				break;

			case 'q':
			case 'x':
				System.out.println("Bye.");
				System.exit(0);

			default:
				System.out.println("Invalid Entry.");
				break;
			}
		}
	}
}
