package chapter4;

public class PriorityQueue {
	private int maxSize;
	private long[] queArray;
	private int nItems;

	public PriorityQueue(int s) {
		maxSize = s;
		queArray = new long[maxSize];
		nItems = 0;
	}

	public void insert(long item) {
		int i = 0;
		if (nItems == 0) {
			queArray[nItems++] = item;
		}
		else {
			for (i = nItems - 1; i > -1; --i) {
				if (item > queArray[i]) {
					queArray[i+1] = queArray[i];
				} else {
					break;
				}
			}
			queArray[i + 1] = item;
			nItems++;
		}
	}

	public long remove() {
		return queArray[--nItems];
	}

	public long peekMin() {
		return queArray[nItems-1];
	}

	public boolean isEmpty() {
		return nItems == 0;
	}

	public boolean isFull() {
		return nItems == maxSize;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int qSize = 9;
		PriorityQueue q = new PriorityQueue(qSize);

		for (int i = 0; i < qSize; ++i) {
			long l = (long) (Math.random() * 100) + 1;
			System.out.print(l + " ");
			q.insert(l);
		}
		System.out.println();

		System.out.println(q.isFull());

		System.out.println(q.peekMin());

		System.out.println(q.remove());

		System.out.println(q.isEmpty());

		System.out.println(q.isFull());	}
}
