package chapter12Heaps;

import java.util.Scanner;

public class HeapArray {
	private class Node {
		private int key;

		public Node(int key) {
			this.key = key;
		}

		public int getKey() {
			return key;
		}

		public void setKey(int key) {
			this.key = key;
		}
	}
	private Node[] heapArray;
	private int maxSize;
	private int currentSize;

	public HeapArray(int maxSize) {
		this.maxSize = maxSize;
		currentSize = 0;
		heapArray = new Node[maxSize];
	}

	private void trickleUp(int index) {
		int parent = (index - 1) / 2;
		Node bottom = heapArray[index];

		while (index > 0 && heapArray[parent].getKey() < bottom.getKey() ) {
			heapArray[index] = heapArray[parent];
			index = parent;
			parent = (parent - 1) / 2;
		}

		heapArray[index] = bottom;
	}

	private void trickleDown(int index) {
		int largerChild;
		Node top = heapArray[index];

		while (index < currentSize / 2) {
			int leftChild = 2 * index + 1;
			int rightChild = leftChild + 1;

			/* Find the largest child. */
			if (rightChild < currentSize &&
					heapArray[leftChild].getKey() < heapArray[rightChild].getKey())
				largerChild = rightChild;
			else
				largerChild = leftChild;

			/* Check if top >= larger child. */
			if (top.getKey() >= heapArray[largerChild].getKey())
				break;

			heapArray[index] = heapArray[largerChild];
			index = largerChild;
		}

		heapArray[index] = top;
	}

	public boolean isEmpty() {
		return currentSize == 0;
	}

	public boolean insert(int key) {
		if (currentSize == maxSize)
			return false;

		heapArray[currentSize] = new Node(key);
		trickleUp(currentSize);
		++currentSize;

		return true;
	}

	public Node remove() {
		Node root = heapArray[0];

		heapArray[0] = heapArray[--currentSize];
		trickleDown(0);

		return root;
	}

	public boolean change(int index, int newValue) {
		if (index < 0 || index >= currentSize)
			return false;

		int oldValue = heapArray[index].getKey();
		heapArray[index].setKey(newValue);

		if (oldValue < newValue)
			trickleUp(index);
		else
			trickleDown(index);

		return true;
	}

	public void displayHeap() {
		System.out.print("heapArray: ");
		for (int i = 0; i < currentSize; ++i) {
			if (heapArray[i] != null)
				System.out.printf("%d ", heapArray[i].getKey());
			else
				System.out.print("-- ");
		}
		System.out.println();

		/* Heap formatting. */

		int nBlanks = 32, itemsPerRow = 1, column = 0, i = 0;
		String dots = "................................";
		System.out.println(dots + dots);

		while (currentSize > 0) {
			if (column == 0)
				for (int j = 0; j < nBlanks; ++j)
					System.out.print(" ");

			System.out.print(heapArray[i].getKey());

			/* Check for done */
			if (++i == currentSize)
				break;

			if (++column == itemsPerRow) {
				nBlanks /= 2;
				itemsPerRow *= 2;
				column = 0;
				System.out.println();
			} else
				for (int j = 0; j < nBlanks * 2 - 2; ++j)
					System.out.print(" ");
		}

		System.out.println("\n" + dots + dots);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int key, newKey;
		HeapArray h = new HeapArray(31);
		Scanner s = new Scanner(System.in);

		h.insert(70);
		h.insert(40);
		h.insert(50);
		h.insert(20);
		h.insert(100);
		h.insert(80);
		h.insert(30);
		h.insert(10);
		h.insert(90);

		while (true) {
			System.out.print("Select from one of the following actions:\n");
			System.out.print("(d)isplay, (i)nsert, (r)emove, (f)ind\n");

			char choice = s.next().toLowerCase().charAt(0);

			switch (choice) {
			case 'd':
				h.displayHeap();
				break;

			case 'i':
				System.out.print("Enter the value to insert\n");
				key = s.nextInt();
				if (!h.insert(key))
					System.out.println("Heap is full, failed insertion.");
				break;

			case 'r':
				if (!h.isEmpty())
					h.remove();
				else
					System.out.println("Can't remove; heap empty.");
				break;

			case 'c':
				System.out.print("Enter current index of item: ");
				key = s.nextInt();
				System.out.println("Enter the new key: ");
				newKey = s.nextInt();

				if (!h.change(key, newKey))
					System.out.println("Invalid index");
				break;

			default:
				System.out.println("Invalid entry.");
				break;
			}
		}
	}

}
