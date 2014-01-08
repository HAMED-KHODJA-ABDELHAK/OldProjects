package chapter5LinkList;

class Link2 {
	public long dData;
	public Link2 next;

	public Link2(long dd) {
		dData = dd;
	}

	public void displayLink() {
		System.out.print(dData + " ");
	}
}

class LinkList {
	private Link2 first;

	public LinkList() {
		first = null;
	}

	public Link2 getFirst() {
		return first;
	}

	public void setFirst(Link2 l) {
		first = l;
	}

	public boolean isEmpty() {
		return first == null;
	}

	public ListIterator getIterator() {
		return new ListIterator(this);
	}

	public void displayList() {
		Link2 current = first;

		while (current != null) {
			current.displayLink();
			current = current.next;
		}

		System.out.println();
	}
}

class ListIterator {
	private Link2 current;
	private Link2 previous;
	private LinkList ourList;

	public ListIterator(LinkList list) {
		ourList = list;
		reset();
	}

	public void reset() {
		current = ourList.getFirst();
		previous = null;
	}

	public boolean atEnd() {
		return current.next == null;
	}

	public void nextLink() {
		previous = current;
		current = current.next;
	}

	public Link2 getCurrent() {
		return current;
	}

	public void insertAfter(long dd) {
		Link2 newLink = new Link2(dd);

		if (ourList.isEmpty()) {
			ourList.setFirst(newLink);
			current = newLink;
		} else {
			newLink.next = current.next;
			current.next = newLink;
			nextLink();
		}
	}

	public void insertBefore(long dd) {
		Link2 newLink = new Link2(dd);

		if (ourList.isEmpty()) {
			newLink.next = ourList.getFirst();
			ourList.setFirst(newLink);
			reset();
		} else {
			newLink.next = current;
			previous.next = newLink;
			current = newLink;
		}
	}

	public long deleteCurrent() {
		long deleted = current.dData;

		/* Beginning of list */
		if (previous == null) {
			ourList.setFirst(current.next);
			reset();
		} else {
			previous.next = current.next;
			if (atEnd())
				reset();
			else
				current = current.next;
		}

		return deleted;
	}
}

public class Iterator {
	public static void main(String[] args) {
		LinkList list = new LinkList();
		ListIterator iter1 = list.getIterator();

		iter1.insertAfter(20);
		iter1.insertAfter(40);
		iter1.insertAfter(80);
		iter1.insertBefore(60);

		list.displayList();

		iter1.reset();
		iter1.nextLink();
		iter1.deleteCurrent();

		list.displayList();

	}
}
