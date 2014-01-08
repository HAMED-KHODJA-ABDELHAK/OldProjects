package chapter4;

/**
 * Queue class that doesn't store it's own size.
 * It is a good example.
 * @author starcraftman
 *
 */
public class QueueNoItems {

	private int maxSize;
	private long[] queArray;
	private int front;
	private int rear;

	public QueueNoItems(int s) {
		maxSize = s+1;
		queArray = new long[maxSize];
		front = 0;
		rear = -1;
	}

	public void insert(long i) {
		if (rear == maxSize -1)
			rear = -1;
		queArray[++rear] = i;
	}

	public long remove() {
		long l = queArray[front++];
		if (front == maxSize)
			front = 0;
		return l;
	}

	public long peek() {
		return queArray[front];
	}

	public boolean isEmpty() {
		return (rear+1 == front) || (front + maxSize - 1 == rear);
	}

	public boolean isFull() {
		return (rear + 3 == front) || (front + maxSize - 3 == rear);
	}

	public int size() {
		if (rear >= front)
			return rear - front +1;
		else
			return (maxSize - front) + (rear + 1);
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		QueueNoItems q = new QueueNoItems(6);

		System.out.println(q.isEmpty());

		for (int i = 0; i < 6; ++i) {
			long l = (long) Math.floor(Math.random() * 100);
			System.out.print(l + " ");
			q.insert(l);
		}

		System.out.println(q.peek());

		System.out.println(q.remove());

		System.out.println(q.size());

		System.out.println(q.isFull());
	}

}
