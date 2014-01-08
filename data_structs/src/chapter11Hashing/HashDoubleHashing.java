package chapter11Hashing;

import java.util.Scanner;

class DataItem2 {
	private int iData;

	public DataItem2(int i) {
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

class HashTable2 {
	private DataItem2[] hashArray;
	private int arraySize;
	private DataItem2 deletedItem;

	public HashTable2(int size) {
		arraySize = size;
		hashArray = new DataItem2[arraySize];
		deletedItem = new DataItem2(-1);
	}

	public void displayTable() {
		System.out.print("Table: ");

		for (DataItem2 d : hashArray) {
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

	public int hashFunc2(int key) {
		return 13 - (key % 13);
	}

	public void insert(DataItem2 item) {
		int hashVal = hashFunc(item.getKey());
		int stepSize = hashFunc2(item.getKey());

		while (hashArray[hashVal] != null && hashArray[hashVal].getKey() != -1) {
			hashVal = (hashVal + stepSize) % arraySize;
		}

		hashArray[hashVal] = item;
	}

	public DataItem2 delete(int key) {
		int hashVal = hashFunc(key);
		int stepSize = hashFunc2(key);

		while (hashArray[hashVal] != null) {
			if (hashArray[hashVal].getKey() == key) {
				DataItem2 temp = hashArray[hashVal];
				hashArray[hashVal] = deletedItem;
				return temp;
			}

			hashVal = (hashVal + stepSize) % arraySize;
		}

		return null;
	}

	public DataItem2 find(int key) {
		int hashVal = hashFunc(key);
		int stepSize = hashFunc2(key);

		while (hashArray[hashVal] != null) {
			if (hashArray[hashVal].getKey() == key)
				return hashArray[hashVal];

			hashVal = (hashVal + stepSize) % arraySize;
		}

		return null;
	}
}

public class HashDoubleHashing {
	public static void main(String[] args) {
		final int KEYS_PER_CELL = 10;
		int size, n, key;
		Scanner s = new Scanner(System.in);

		System.out.println("Enter the size of the hash table: ");
		size = s.nextInt();
		System.out.println("Enter the initial number of items: ");
		n = s.nextInt();

		HashTable2 h = new HashTable2(size);

		for (int i = 0; i < n; ++i) {
			key = (int) (Math.random() * KEYS_PER_CELL * size);
			h.insert(new DataItem2(key));
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
				h.insert(new DataItem2(key));
				break;

			case 'r':
				System.out.println("Enter key to remove: ");
				key = s.nextInt();
				h.delete(key);
				break;

			case 'f':
				System.out.println("Enter the key value to find: ");
				key = s.nextInt();
				DataItem2 d = h.find(key);

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
