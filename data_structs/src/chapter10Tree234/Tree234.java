package chapter10Tree234;

import java.util.Scanner;

class DataItem {
	public long dData;

	public DataItem(long dd) {
		dData = dd;
	}

	@Override
	public String toString() {
		return String.format("/%d", dData);
	}
}

class Node {
	private static final int MAX_CHILDREN = 4;
	private static final int MAX_DATA = 3;
	private int numItems;
	private Node parent;
	private Node childArray[] = new Node[MAX_CHILDREN];
	private DataItem itemArray[] = new DataItem[MAX_DATA];

	/**
	 * Connect a child into this Node at the given index number.
	 * @param childNum
	 * @param child
	 */
	public void connectChild(int childNum, Node child) {
		childArray[childNum] = child;

		if (child != null)
			child.parent = this;
	}

	/**
	 * Remove a child from the node that has the given num.
	 * @param childNum
	 * @return
	 */
	public Node disconnectChild(int childNum) {
		Node temp = childArray[childNum];
		childArray[childNum] = null;
		return temp;
	}

	/**
	 * Find's a given item's index within the node's itemArray.
	 *
	 * @param key Long key that searching for.
	 * @return Index of item if found, else -1.
	 */
	public int findItem(long key) {
		for (int i = 0; i < MAX_DATA; ++i) {
			if (itemArray[i] == null)
				break;
			else if (itemArray[i].dData == key)
				return i;
		}

		return -1;
	}

	/**
	 * Method to insert a new data item into the given node.
	 *
	 * @param newItem
	 * @return
	 */
	public int insertItem(DataItem newItem) {
		numItems++;
		long newKey = newItem.dData;

		/* Move across the array from right to left. */
		for (int i = MAX_DATA - 1; i >= 0; --i) {

			/* Item doesn't exist, move left.*/
			if (itemArray[i] == null)
				continue;
			/* Item exists. */
			else {
				long itsKey = itemArray[i].dData;

				/* Move it right */
				if (newKey < itsKey)
					itemArray[i+1] = itemArray[i];
				/* Insert item to right of this itme */
				else {
					itemArray[i+1] = newItem;
					return i+1;
				}
			}
		}

		/* Nothing there. */
		itemArray[0] = newItem;
		return 0;
	}

	/**
	 * Remove the largest item from the itemArray.
	 *
	 * @return The largest now removed item.
	 */
	public DataItem removeItem() {
		DataItem temp = itemArray[numItems-1];
		itemArray[--numItems] = null;
		return temp;
	}

	/**
	 * Method to display a node's item array.
	 * Format is "/x/y/z/" for as many items exist.
	 */
	@Override
	public String toString() {
		String temp = "";
		for (int i = 0; i < numItems; ++i)
			temp += itemArray[i].toString();
		temp += "/";

		return temp;
	}

	public boolean isLeaf() {
		return childArray[0] == null;
	}

	public boolean isFull() {
		return numItems == MAX_DATA;
	}

	public Node getChild(int childNum) {
		return childArray[childNum];
	}

	public Node getParent() {
		return parent;
	}

	public int getNumItems() {
		return numItems;
	}

	public DataItem getItem(int index) {
		return itemArray[index];
	}

	public static int getMaxData() {
		return MAX_DATA;
	}

	public static int getMaxChildren() {
		return MAX_CHILDREN;
	}
}

/**
 * Main logic class, contains methods for operating on whole 234 tree.
 *
 */
public class Tree234 {
	private Node root = new Node();

	public Node getRoot() {
		return root;
	}

	/**
	 * Search the whole tree to find the item with key.
	 * @param key
	 * @return
	 */
	public int find(long key) {
		Node curNode = root;
		int childNumber;

		/* Find key in current node or get next child to search */
		while (true) {
			childNumber = curNode.findItem(key);

			if (childNumber != -1)
				return childNumber;
			else if (curNode.isLeaf())
				return -1;
			else
				curNode = getNextChild(curNode, key);
		}
	}

	/**
	 * Insert a given data value within the tree.
	 *
	 * @param dValue
	 */
	public void insert(long dValue) {
		Node curNode = root;
		DataItem tempItem = new DataItem(dValue);

		/* Seek to correct node, splitting on full Nodes. */
		while (true) {
			if (curNode.isFull()) {
				split(curNode);
				curNode = curNode.getParent();

				curNode = getNextChild(curNode, dValue);
			}

			else if (curNode.isLeaf())
				break;

			else
				curNode = getNextChild(curNode, dValue);
		}

		curNode.insertItem(tempItem);
	}

	/**
	 * Take a full node and split it into separate nodes.
	 * @param thisNode
	 */
	public void split(Node thisNode) {
		DataItem itemB, itemC;
		Node parent, child2, child3;
		int itemIndex;

		itemC = thisNode.removeItem();
		itemB = thisNode.removeItem();
		child2 = thisNode.disconnectChild(2);
		child3 = thisNode.disconnectChild(3);
		Node newRight = new Node();

		/* Splitting root node */
		if (thisNode == root) {
			root = new Node();
			parent = root;
			root.connectChild(0, thisNode);
		}
		else
			parent = thisNode.getParent();

		/* Manipulate parent */
		itemIndex = parent.insertItem(itemB);
		int n = parent.getNumItems();

		/* Move parent's connections one child to right. */
		for (int i = n-1; i > itemIndex; --i) {
			Node temp = parent.disconnectChild(i);
			parent.connectChild(i+1, temp);
		}

		/* Connect new node to right of original. */
		parent.connectChild(itemIndex+1, newRight);

		newRight.insertItem(itemC);
		newRight.connectChild(0, child2);
		newRight.connectChild(1, child3);
	}

	/**
	 * Based on the current node, find the next child to visit to get to theValue.
	 * Precondition: Node isn't empty, isn't full and isn't a leaf.
	 *
	 * @param theNode This node.
	 * @param theValue The desired key.
	 * @return Next child.
	 */
	public Node getNextChild(Node theNode, long theValue) {
		int i;
		int numItems = theNode.getNumItems();

		/* Find if value less than node, then take child */
		for (i = 0; i < numItems; ++i) {
			if (theValue < theNode.getItem(i).dData)
				return theNode.getChild(i);
		}

		/* Must be greater */
		return theNode.getChild(i);
	}

	/**
	 * Get the lowest key within the 234 tree.
	 * @return
	 */
	public long getMinimum() {
		Node curNode = root;

		while (true) {
			if (curNode.getChild(0) == null)
				break;

			curNode = curNode.getChild(0);
		}

		return curNode.getItem(0).dData;
	}

	public long getMaximum() {
		Node curNode = root;
		int i = 0;

		/* Find right most child and take it, if leaf break. */
		while (true) {
			for (i = Node.getMaxChildren()-1; i >= 0; --i)
				if (curNode.getChild(i) != null)
					break;

			if (curNode.isLeaf())
				break;
			else
				curNode = curNode.getChild(i);
		}

		/* Find right most data, max */
		for (i = Node.getMaxData()-1; i >= 0; --i) {
			if (curNode.getItem(i) != null)
				break;
		}

		return curNode.getItem(i).dData;
	}

	public String inOrderTraversal(Node curNode, String list) {
		if (curNode != null) {

			for (int i = 0; i < Node.getMaxData(); ++i) {
				list += inOrderTraversal(curNode.getChild(i), "");

				DataItem d = curNode.getItem(i);
				if (d != null)
					list += d;
			}

			list += inOrderTraversal(curNode.getChild(3), "");

			return list;
		}

		return "";
	}

	/**
	 * Display the tree graphically.
	 */
	public void displayTree() {
		recDisplayTree(root, 0, 0);
	}

	/**
	 * Recursively display this levels elements and then call all childrent to display.
	 *
	 * @param thisNode
	 * @param level
	 * @param childNumber
	 */
	public void recDisplayTree(Node thisNode, int level, int childNumber) {
		System.out.printf("level=%d child=%d %s\n", level, childNumber, thisNode);

		int numItems = thisNode.getNumItems();
		for (int i = 0; i < numItems+1; ++i) {
			Node nextNode = thisNode.getChild(i);

			if (nextNode != null)
				recDisplayTree(nextNode, level+1, i);
			else
				return;
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		long value;
		Tree234 t = new Tree234();
		Scanner s = new Scanner(System.in);

		t.insert(50);
		t.insert(40);
		t.insert(60);
		t.insert(30);
		t.insert(70);
		t.insert(10);
		t.insert(80);
		t.insert(20);
		t.insert(90);

		boolean run = true;
		while (run) {
			System.out.print("Choose from the following commands:\n");
			System.out.print("(d)isplay, (i)nsert, (m)in, (f)ind (q)uit.\n");

			char c = s.next().toLowerCase().charAt(0);

			switch (c) {
			case 'd':
				t.displayTree();
				break;

			case 'i':
				System.out.print("Enter value to insert: \n");
				value = s.nextLong();
				t.insert(value);
				break;

			case 'f':
				System.out.print("Enter value to find: \n");
				value = s.nextLong();

				int found = t.find(value);
				if (found != -1)
					System.out.printf("Found %d\n", value);
				else
					System.out.printf("Could not find %d\n", value);
				break;

			case 'm':
				System.out.printf("The current minimum is: %d\n", t.getMinimum());
				break;

			case 'x':
				System.out.printf("The max value is: %d\n", t.getMaximum());
				break;

			case 't':
				System.out.print("In order list: \n");
				String list = "";
				System.out.println(t.inOrderTraversal(t.getRoot(), list));
				break;

			case 'q':
				run = false;
				s.close();
				break;

			default:
				System.out.print("Invalid entry\n");
				break;
			}
		}
	}
}
