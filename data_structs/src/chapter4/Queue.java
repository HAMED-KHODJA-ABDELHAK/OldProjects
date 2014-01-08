package chapter4;

public class Queue {
	private int maxSize;
	private long[] queArray;
	private int front;
	private int rear;
	private int nItems;

	public Queue(int s) {
		maxSize = s;
		queArray = new long[maxSize];
		front = 0;
		rear = -1;
		nItems = 0;
	}

	public void insert(long j) {
		if (rear == maxSize-1)
			rear = -1;
		queArray[++rear] = j;
		nItems++;
	}

	public long remove() {
		long temp = queArray[front++];
		if (front == maxSize)
			front = 0;
		nItems--;

		return temp;
	}

	public long peekFront() {
		return queArray[front];
	}

	public boolean isEmpty() {
		return nItems == 0;
	}

	public boolean isFull() {
		return nItems == maxSize;
	}

	/**
	 * Size function.
	 * @return
	 */
	public int size() {
		return nItems;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Queue queue = new Queue(5);

		queue.insert(10);
		queue.insert(20);
		queue.insert(30);
		queue.insert(40);

		queue.remove();
		queue.remove();
		queue.remove();

		for (int i = 50; i < 80; i += 10 )
			queue.insert(i);

		queue.size();

		while (!queue.isEmpty()) {
			long n = queue.remove();
			System.out.print(n + " ");
		}
		System.out.println();
	}
}
