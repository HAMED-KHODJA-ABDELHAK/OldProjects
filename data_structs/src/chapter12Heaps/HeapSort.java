package chapter12Heaps;

import java.util.Scanner;

public class HeapSort {
	private class Node {
		private int key;
		private Node leftChild;
		private Node rightChild;
		private Node parent;

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
	private int numItems;
	private Node root;
	private static String[] heapStr;

	public HeapSort() {
		root = null;
		numItems = 0;
	}

	private void trickleUp(Node n) {
		int key = n.key;
		Node current = n;
		Node parent = n.parent;

		while (current != root && parent.key < key) {
			current.key = parent.key;
			current = parent;
			parent = current.parent;
		}

		current.key = key;
	}

	private void trickleDown(int i) {
		Node current = root;
		Node largerChild = null;

		if (i != 0) {
			/* Get path to add to new node. */
			int[] path = pathToNode(i + 1);
			for (int j = 0; j < path.length; ++j) {
				if (path[j] == 1)
					current = current.rightChild;
				else
					current = current.leftChild;
			}
		}
		int top = current.getKey();

		while (current != null) {
			if (current.rightChild != null &&
					current.leftChild.getKey() < current.rightChild.getKey())
				largerChild = current.rightChild;
			else
				largerChild = current.leftChild;

			if (largerChild == null || top >= largerChild.getKey())
				break;

			current.setKey(largerChild.getKey());
			current = largerChild;
		}

		current.setKey(top);
	}

	private int[] pathToNode(int index) {
		/* Calculates the exact amount of space needed to store path. */
		int arraySize = (int) (Math.log(index) / Math.log(2));
		int i = arraySize - 1;
		int[] path = new int[arraySize];

		/* Convert index to binary, ignore first position. */
		while (index >= 2) {
			path[i--] = index % 2;
			index /= 2;
		}

		return path;
	}

	public boolean isEmpty() {
		return numItems == 0;
	}

	public boolean insert(int key) {
		Node newNode = new Node(key);
		Node parent = null;
		Node current = root;

		if (root == null) {
			root = newNode;
			++numItems;
			return true;
		}

		/* Get path to add to new node. */
		int[] path = pathToNode(numItems + 1);
		for (int i = 0; i < path.length; ++i) {
			parent = current;
			if (path[i] == 1)
				current = current.rightChild;
			else
				current = current.leftChild;
		}

		/* Insert node and set its parent. */
		if (path[path.length-1] == 1)
			parent.rightChild = newNode;
		else
			parent.leftChild = newNode;
		newNode.parent = parent;

		/* Swap elements until keys ordered properly. */
		trickleUp(newNode);
		++numItems;

		return true;
	}

	public Node remove() {
		Node temp = new Node(root.getKey());
		Node parent = null;
		Node current = root;

		if (numItems == 1) {
			root = null;
			--numItems;
			return temp;
		}

		/* Get to last node. Swap to root. */
		int[] path = pathToNode(numItems);
		for (int i = 0; i < path.length; ++i) {
			parent = current;
			if (path[i] == 1)
				current = current.rightChild;
			else
				current = current.leftChild;
		}

		/* Set root to bottom key, then remove. Trickle down new root. */
		root.setKey(current.getKey());
		if (path[path.length-1] == 1) {
			parent.rightChild = null;
		}
		else {
			parent.leftChild = null;
		}

		/* Swap elements until keys ordered properly. */
		trickleDown(0);
		--numItems;

		return temp;
	}

	private void recDisplay(Node n, int level) {
		if (n != null) {
			heapStr[level] += n.key + " ";
			level++;
			recDisplay(n.leftChild, level);
			recDisplay(n.rightChild, level);
		}
	}

	public void displayHeap() {
		int levels = pathToNode(numItems).length + 1;
		heapStr = new String[levels];
		for (int i = 0; i < levels; ++i)
			heapStr[i] = "";

		recDisplay(root, 0);

		for (int i = 0; i < levels; ++i)
			System.out.println(heapStr[i]);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		HeapSort h = new HeapSort();
		Scanner s = new Scanner(System.in);

		System.out.printf("Please enter a number\n");
		int size = s.nextInt();

		long startTime = System.currentTimeMillis();

		for (int i = 0; i < size; ++i)
			h.insert((int) (Math.random() * size * 10));

		/* Heapify. */
//		for (int i = (h.numItems / 2) - 1; i >= 0; --i)
//			h.trickleDown(i);
//
		for (int i = 0; i < size; ++i)
			h.remove();

		long endTime = System.currentTimeMillis();
		System.out.printf("Done. Time: %d\n", (endTime - startTime));
	}
}
