package chapter8BinTrees;

import java.util.Scanner;
import java.util.Stack;

/**
 * A rough binary tree implementation.
 * @author starcraftman
 *
 */
public class BinaryTree1 {
	private class Node {
		int iData;
		double dData;
		Node leftChild;
		Node rightChild;

		public Node(int iD, double dD) {
			iData = iD;
			dData = dD;
		}

		/* Display node in increasing order. */
		public void displayNode() {
			System.out.printf("{%d, %.2f}", iData, dData);
		}
	}
	private Node root;

	public BinaryTree1() {
		root = null;
	}

	public Node find(int key) {
		Node current = root;

		while (current.iData != key) {
			if (key < current.iData)
				current = current.leftChild;
			else
				current = current.rightChild;
			if (current == null)
				return null;
		}

		return current;
	}

	/**
	 * Insert a node created from an id and data.
	 * @param id
	 * @param data
	 */
	public void insert(int id, double data) {
		Node node = new Node(id, data);
		Node current = root, parent = null;

		if (root == null) {
			root = node;
			return ;
		}

		while (current != null) {
			parent = current;

			/* Do we go right? */
			if (id < current.iData) {
				current = current.leftChild;

				/* Did we hit null leaf? Insert. */
				if (current == null) {
					parent.leftChild = node;
					return ;
				}
			} else {
				current = current.rightChild;

				/* We must add to left leaf. */
				if (current == null) {
					parent.rightChild = node;
					return ;
				}
			}
		}
	}

	public boolean delete(int id) {
		Node current = root, parent = null;
		boolean isLeftChild = true;

		if (root == null)
			return false;

		/* Current should be set to point to node for deletion. */
		while (current.iData != id) {
			parent = current;

			if (id < current.iData) {
				current = current.leftChild;
				isLeftChild = true;
			} else {
				current = current.rightChild;
				isLeftChild = false;
			}
			if (current == null)
				return false;
		}

		/* No child case. */
		if (current.leftChild == null && current.rightChild == null) {
			if (current == root)
				root = null;
			if (isLeftChild)
				parent.leftChild = null;
			else
				parent.rightChild = null;
		}

		/* 1 Child */
		else if (current.rightChild == null) {
			if (current == root)
				root = current.leftChild;
			else if (isLeftChild)
				parent.leftChild = current.leftChild;
			else
				parent.rightChild = current.leftChild;
		}

		else if (current.leftChild == null) {
			if (current == root)
				root = current.rightChild;
			else if (isLeftChild)
				parent.leftChild = current.rightChild;
			else
				parent.rightChild = current.rightChild;
		}

		/* 2 Children, find min past right and use to replace. */
		else {
			Node sucessor = getSucessor(current);

			if (current == root)
				root = sucessor;
			else if (isLeftChild)
				parent.leftChild = sucessor;
			else
				parent.rightChild = sucessor;

			/* Connect sucessor to left child of the deleted node. */
			sucessor.leftChild = current.leftChild;
		}

		return true;
	}

	private Node getSucessor(Node delNode) {
		Node sucessorParent = delNode, sucessor = delNode;
		Node current = delNode.rightChild;

		/* Go left until at minimum */
		while (current != null) {
			sucessorParent = sucessor;
			sucessor = current;
			current = current.leftChild;
		}

		/* Special case if sucessor found on left descendant chain. */
		if (sucessor != delNode.rightChild) {
			sucessorParent.leftChild = sucessor.rightChild;
			sucessor.rightChild = delNode.rightChild;
		}

		return sucessor;
	}

	public void traverse(int traverseType) {
		switch(traverseType) {
		case 0:
			System.out.print("\nPreorder traversal: ");
			preOrder(root);
			break;
		case 1:
			System.out.print("\nInorder traversal: ");
			inOrder(root);
			break;
		case 2:
			System.out.print("\nPostorder traversal: ");
			postOrder(root);
			break;
		default:
			System.out.print("Be more careful next time!\n");
			break;
		}

		System.out.println();
	}

	private void preOrder(Node localRoot) {
		if (localRoot != null) {
			System.out.printf("%d ", localRoot.iData);
			preOrder(localRoot.leftChild);
			preOrder(localRoot.rightChild);
		}
	}

	private void inOrder(Node localRoot) {
		if (localRoot != null) {
			inOrder(localRoot.leftChild);
			System.out.printf("%d ", localRoot.iData);
			inOrder(localRoot.rightChild);
		}
	}

	private void postOrder(Node localRoot) {
		if (localRoot != null) {
			postOrder(localRoot.leftChild);
			postOrder(localRoot.rightChild);
			System.out.printf("%d ", localRoot.iData);
		}
	}

	public void displayTree() {
		Stack<Node> globalStack = new Stack<Node>();
		globalStack.push(root);
		int nBlanks = 64;
		boolean isRowEmpty = false;

		for (int i = 0; i < 2 * nBlanks; ++i)
			System.out.print(".");
		System.out.println();

		while (isRowEmpty == false) {
			Stack<Node> localStack = new Stack<Node>();
			isRowEmpty = true;

			for (int i = 0; i < nBlanks; ++i)
				System.out.print(" ");

			while (globalStack.isEmpty() == false) {
				Node temp = globalStack.pop();

				if (temp != null) {
					System.out.print(temp.iData);
					localStack.push(temp.leftChild);
					localStack.push(temp.rightChild);

					if (temp.leftChild != null || temp.rightChild != null)
						isRowEmpty = false;
				} else {
					System.out.print("--");
					localStack.push(null);
					localStack.push(null);
				}

				for (int i = 0; i < nBlanks * 2 - 2; ++i)
					System.out.print(" ");
			}

			System.out.println();
			nBlanks /= 2;

			while (localStack.isEmpty() == false)
				globalStack.push(localStack.pop());
		}

		System.out.println();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		BinaryTree1 b = new BinaryTree1();
		int key;

		b.insert(50, 1.5);
		b.insert(25, 1.7);
		b.insert(75, 1.9);
		b.insert(12, 1.5);
		b.insert(37, 1.2);
		b.insert(43, 1.7);
		b.insert(30, 1.5);
		b.insert(33, 1.2);
		b.insert(87, 1.7);
		b.insert(93, 1.5);
		b.insert(97, 1.5);

		Scanner s = new Scanner(System.in);

		boolean run = true;
		while (run) {
			System.out.print("Choose from among the following commands:\n");
			System.out.print("(d)isplay, (i)nsert, (f)ind, (r)emove, (t)raverse or (q)uit.\n");
			char c = s.next().toLowerCase().charAt(0);

			switch (c) {
			case 'd':
				b.displayTree();
				break;

			case 'i':
				System.out.print("Enter key of the node to insert: \n");
				key = s.nextInt();
				System.out.print("Enter data (double) of the node to insert: \n");
				double data = s.nextDouble();

				b.insert(key, data);
				break;

			case 'f':
				System.out.print("Enter the key to find: \n");
				key = s.nextInt();

				Node found = b.find(key);
				if (found != null) {
					System.out.print("Found: ");
					found.displayNode();
					System.out.println();
				} else {
					System.out.printf("Could not find key %d \n", key);
				}
				break;

			case 'r':
				System.out.print("Enter key to delete: \n");
				key = s.nextInt();

				boolean didDelete = b.delete(key);
				if (didDelete)
					System.out.printf("Deleted node with key %d \n", key);
				else
					System.out.printf("Could not delete %d\n", key);
				break;

			case 't':
				System.out.print("Choose from preOrder (0), inOrder (1) and postOrder (2) traversal.\n");
				int val = s.nextInt();

				b.traverse(val);
				break;

			case 'q':
				run = false;
				s.close();
				break;

			default:
				System.out.print("Invalid entry\n");
			}
		}
	}
}
