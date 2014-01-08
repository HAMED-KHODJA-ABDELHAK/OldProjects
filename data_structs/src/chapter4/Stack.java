package chapter4;

public class Stack {
	private int maxSize;
	private long[] stackArray;
	private int top;

	public Stack(int s) {
		maxSize = s;
		stackArray = new long[maxSize];
		top = -1;
	}

	public void push(long l) {
		stackArray[++top] = l;
	}

	public long pop() {
		return stackArray[top--];
	}

	public long peek() {
		return stackArray[top];
	}

	public boolean isEmpty() {
		return top == -1;
	}

	public boolean isFull() {
		return top == maxSize;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Stack s = new Stack(6);

		s.push(444);
		s.push(212);
		s.push(443);
		s.push(892);
		s.push(111);

		while (!s.isEmpty())
			System.out.println(s.pop() + " ");
		System.out.println();
	}

}
