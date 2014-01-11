package chapter12Heaps;

import java.util.Scanner;

public class HeapTree <T> {
	private class Node {
		private int key;
		private T obj;
		private Node leftChild;
		private Node rightChild;
		private Node parent;

		public Node(int key, T obj) {
			this.key = key;
			this.obj = obj;
		}

		public int getKey() {
			return key;
		}

		public void setKey(int key) {
			this.key = key;
		}

		public T getObj() {
			return obj;
		}

		public void setObj(T obj) {
			this.obj = obj;
		}
	}
	private int numItems;
	private Node root;
	private static String[] heapStr;

	public HeapTree() {
		root = null;
		numItems = 0;
	}

	private void trickleUp(Node n) {
		int nKey = n.getKey();
		T nObj = n.getObj();
		Node current = n;
		Node parent = n.parent;

		/* Move from heap leaf to root, swapping key/obj to parent so
		 * long as key greater than parent. */
		while (current != root && parent.key < nKey) {
			current.setKey(parent.getKey());
			current.setObj(parent.getObj());
			current = parent;
			parent = parent.parent;
		}

		/* Set saved key, object into node. */
		current.setKey(nKey);
		current.setObj(nObj);
	}

	private void trickleDown(Node n) {
		Node current = n;
		Node largerChild = null;
		int nKey = n.getKey();
		T nObj = n.getObj();

		while (current != null) {
			if (current.rightChild != null &&
					current.leftChild.getKey() < current.rightChild.getKey())
				largerChild = current.rightChild;
			else
				largerChild = current.leftChild;

			if (largerChild == null || nKey >= largerChild.getKey())
				break;

			current.setKey(largerChild.getKey());
			current.setObj(largerChild.getObj());
			current = largerChild;
		}

		current.setKey(nKey);
		current.setObj(nObj);
	}

	/**
	 * Path to a given node in the heap. Path is equal to binary string representing
	 * index of heap node, without first bit position.
	 * @param index
	 * @return path to a node.
	 */
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

	public int getSize() {
		return numItems;
	}

	public void clear() {
		while (!isEmpty())
			pop();
	}

	public Node peek() {
		return root;
	}

	public boolean add(int key) {
		Node newNode = new Node(key, null);
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

	public Node pop() {
		Node temp = new Node(root.getKey(), null);
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
		trickleDown(root);
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

		for (int i = 0; i < levels; ++i) {
			System.out.println(heapStr[i]);
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int key;
		HeapTree<Integer> h = new HeapTree<Integer>();
		Scanner s = new Scanner(System.in);

		h.add(70);
		h.add(40);
		h.add(50);
		h.add(20);
		h.add(100);
		h.add(80);
		h.add(30);
		h.add(10);
		h.add(90);

		boolean run = true;
		while (run) {
			System.out.print("Select from one of the following actions:\n");
			System.out.print("(d)isplay, (i)nsert, (r)emove, (q)uit.\n");

			char choice = s.next().toLowerCase().charAt(0);

			switch (choice) {
			case 'd':
				h.displayHeap();
				break;

			case 'i':
				System.out.print("Enter the value to insert\n");
				key = s.nextInt();
				h.add(key);
				break;

			case 'r':
				if (!h.isEmpty())
					System.out.printf("Key removed: %d\n", h.pop().getKey());
				else
					System.out.println("Can't remove; heap empty.");
				break;

			case 'q':
				run = false;
				s.close();
				break;

			default:
				System.out.println("Invalid entry.");
				break;
			}
		}
	}
}
