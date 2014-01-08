package chapter4;

public class CharStack {
	private int size;
	private char[] stackArray;
	private int top;

	public CharStack(int max) {
		size = max;
		stackArray = new char[max];
		top = -1;
	}

	public void push(char c) {
		stackArray[++top] = c;
	}

	public char pop() {
		return stackArray[top--];
	}

	public char peek() {
		return stackArray[top];
	}

	public boolean isEmpty() {
		return top == -1;
	}

	public boolean isFull() {
		return top == size;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		String s = "Futurama is great!";
		ReverseWord r = new ReverseWord(s);

		System.out.println(r.doReverse());
	}

}

class ReverseWord {
	private String input;
	private String output;

	public ReverseWord(String input) {
		this.input = input;
	}

	public String doReverse() {
		int stackSize = input.length();
		CharStack s = new CharStack(stackSize);

		for (int i = 0; i < input.length(); ++i) {
			s.push(input.charAt(i));
		}

		output = "";
		while (!s.isEmpty()) {
			output += s.pop();
		}

		return output;
	}
}
