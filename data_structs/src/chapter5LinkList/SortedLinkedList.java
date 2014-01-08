package chapter5LinkList;

class Link {
	public long dData;
	public Link next;

	public Link(long dd) {
		dData = dd;
		next = null;
	}

	public void displayLink() {
		System.out.printf(" %d", dData);
	}
}

public class SortedLinkedList {
	private Link first;

	public SortedLinkedList() {
		first = null;
	}

	public boolean isEmpty() {
		return first == null;
	}

	public void insert(long key) {
		Link newLink = new Link(key);
		Link previous = null;
		Link current = first;

		/* Iterate list until found spot */
		while (current != null && key > current.dData) {
			previous = current;
			current = current.next;
		}

		/* If begin, insert first. Else update previous */
		if (previous == null)
			first = newLink;
		else
			previous.next = newLink;

		newLink.next = current;
	}

	public Link remove() {
		Link temp = first;
		first = first.next;
		return temp;
	}

	public void displayList() {
		System.out.print("List (first-->last): ");
		Link current = first;

		while (current != null) {
			current.displayLink();
			current = current.next;
		}

		System.out.println();
	}

	public static void main(String[] args) {
		SortedLinkedList list = new SortedLinkedList();
		list.insert(20);
		list.insert(40);
		list.displayList();

		list.insert(10);
		list.insert(30);
		list.insert(50);

		list.displayList();
		list.remove();
		list.displayList();
	}

}
