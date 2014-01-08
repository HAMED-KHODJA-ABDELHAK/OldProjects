package chapter5LinkList;

public class DoubleLinkList {
	private class Link {
		private long dData;
		private Link next;
		private Link previous;

		public Link(long data) {
			dData = data;
			next = null;
			previous = null;
		}

		public void displayLink() {
			System.out.print(dData + " ");
		}
	}

	private Link first;
	private Link last;

	public DoubleLinkList() {
		first = null;
		last = null;
	}

	/**
	 * Tests whether the list is empty.
	 * @return is the list empty
	 */
	public boolean isEmpty() {
		return first == null;
	}

	public void insertFirst(long dd) {
		Link newLink = new Link(dd);

		if (isEmpty())
			last = newLink;
		else
			first.previous = newLink;

		newLink.next = first;
		first = newLink;
	}

	public void insertLast(long dd) {
		Link newLink = new Link(dd);

		if (isEmpty())
			first = newLink;
		else {
			last.next = newLink;
			newLink.previous = last;
		}

		last = newLink;
	}

	public Link deleteFirst() {
		Link temp = first;

		if (first.next == null)
			last = null;
		else
			first.next.previous = null;

		first = first.next;

		return temp;
	}

	public Link deleteLast() {
		Link temp = last;

		if (last.previous == null)
			first = null;
		else
			last.previous.next = null;

		last = last.previous;
		last.previous = null;

		return temp;
	}

	public boolean insertAfter(long key, long data) {
		Link current = first;
		Link newLink = new Link(data);

		if (isEmpty())
			return false;

		while (current.dData !=  key) {
			current = current.next;

			if (current == null)
				return false;
		}

		if (current.next != null) {
			newLink.next = current.next;
			current.next.previous = newLink;
		}
		newLink.previous = current;
		current.next = newLink;

		if (current == last)
			last = newLink;

		return true;
	}

	public boolean deleteKey(long key) {
		Link current = first;

		if (isEmpty())
			return false;

		while (current.dData != key) {
			current = current.next;

			if (current == null)
				return false;
		}

		if (current == last)
			last = current.previous;
		else
			current.next.previous = current.previous;

		if (current == first)
			first = current.next;
		else
			current.previous.next = current.next;

		return true;
	}

	public void displayForward() {
		System.out.println("List (first --> last): ");

		Link current = first;

		while (current != null) {
			current.displayLink();
			current = current.next;
		}

		System.out.println();
	}

	public void displayBackward() {
		System.out.println("List (last --> first): ");

		Link current = last;

		while (current != null) {
			current.displayLink();
			current = current.previous;
		}

		System.out.println();
	}

	@Override
	public String toString() {
		String contents = "";
		Link link = first;

		while (link != null) {
			contents += link.dData + " ";
			link = link.next;
		}

		return contents;
	}

	public static void main(String[] args) {
		DoubleLinkList l = new DoubleLinkList();

		l.insertFirst(22);
		l.insertFirst(44);
		l.insertFirst(66);

		l.insertLast(11);
		l.insertLast(33);
		l.insertLast(55);

		l.displayForward();
		l.displayBackward();

		l.deleteFirst();
		l.deleteLast();
		l.deleteKey(11);

		l.displayForward();
		l.insertAfter(22, 77);
		l.insertAfter(33, 88);

		l.displayForward();
	}
}
