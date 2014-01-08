package chapter11Hashing;

import java.util.Scanner;

class Link {
	private int key;
	public Link next;

	public Link(int i) {
		key = i;
	}

	public int getKey() {
		return key;
	}

	@Override
	public String toString() {
		return String.format("%d ", key);
	}

	public void displayLink() {
		System.out.print(this);
	}
}

class SortedList {
	private Link first;

	public SortedList() {
		first = null;
	}

	public void insert(Link link) {
		int key = link.getKey();
		Link previous = null, current = first;

		while (current != null && key > current.getKey()) {
			previous = current;
			current = current.next;
		}

		if (previous == null)
			first = link;
		else
			previous.next = link;

		link.next = current;
	}

	public void delete(int key) {
		Link previous = null, current = first;

		while (current != null && key != current.getKey()) {
			previous = current;
			current = current.next;
		}

		if (previous == null)
			first = first.next;
		else
			previous.next = current.next;
	}

	public Link find(int key) {
		Link current = first;

		while (current != null && key >= current.getKey()) {
			if (key == current.getKey())
				return current;
			current = current.next;
		}

		return null;
	}

	public void displayList() {
		Link current = first;

		System.out.print("List (first-->last): ");

		while (current != null) {
			current.displayLink();
			current = current.next;
		}

		System.out.println();
	}
}

class HashTable3 {
	private SortedList[] hashArray;
	private int arraySize;

	public HashTable3(int size) {
		arraySize = size;
		hashArray = new SortedList[size];

		for (int i = 0; i < arraySize; ++i) {
			hashArray[i] = new SortedList();
		}
	}

	public int hashFunc(int key) {
		return key % arraySize;
	}

	public void insert(Link link) {
		int key = link.getKey();
		int hashVal = hashFunc(key);

		hashArray[hashVal].insert(link);
	}

	public void delete(int key) {
		int hashVal = hashFunc(key);
		hashArray[hashVal].delete(key);
	}

	public Link find(int key) {
		int hashVal = hashFunc(key);
		return hashArray[hashVal].find(key);
	}

	public void displayTable() {
		for(int i = 0; i < arraySize; ++i) {
			System.out.printf("%d. ", i);
			hashArray[i].displayList();
		}
	}
}

public class HashSeparateChain {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int KEYS_PER_CELL = 100;
		int size, numItems, key;
		Scanner s = new Scanner(System.in);

		System.out.print("Enter the size of the hash table: \n");
		size = s.nextInt();
		System.out.print("Enter the initial number of items: \n");
		numItems = s.nextInt();

		HashTable3 h = new HashTable3(size);

		for (int i = 0; i < numItems; ++i) {
			key = (int) (Math.random() * KEYS_PER_CELL * size);
			h.insert(new Link(key));
		}

		while (true) {
			System.out.print("Choose from following commands:\n");
			System.out.print("(d)isplay, (i)nsert, (r)emove, (f)ind.\n");
			char choice  = s.next().toLowerCase().charAt(0);

			switch (choice) {
			case 'd':
				h.displayTable();
				break;

			case 'i':
				System.out.print("Enter key to insert:\n");
				key = s.nextInt();
				h.insert(new Link(key));
				break;

			case 'r':
				System.out.print("Enter key to remove:\n");
				key = s.nextInt();
				h.delete(key);
				break;

			case 'f':
				System.out.print("Enter the key of the value to find: \n");
				key = s.nextInt();
				if (h.find(key) != null)
					System.out.print("Found the key.\n");
				else
					System.out.print("Could not locate.\n");
				break;

			default:
				System.out.print("Invalid entry.\n");
				break;
			}
		}
	}

}
