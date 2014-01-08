package chapter4;

import java.util.HashMap;

public class ConvertToPostfix {
	private CharStackPost stack;
	static private HashMap<Character, Integer> map;
	private String input;
	private String output = "";

	static {
		map = new HashMap<Character, Integer>();
		map.put('+', 1);
		map.put('-', 1);
		map.put('*', 2);
		map.put('/', 2);
	}

	public ConvertToPostfix(String in) {
		input = in;
		int stackSize = input.length();
		stack = new CharStackPost(stackSize);
	}

	public String doTrans() {
		for (int i = 0; i < input.length(); ++i) {
			char ch = input.charAt(i);
			System.out.println(stack);
			switch(ch) {
			case '+':
			case '-':
				gotOper(ch, getPrecedence(ch));
				break;
			case '*':
			case '/':
				gotOper(ch, getPrecedence(ch));
				break;
			case '(':
				stack.push(ch);
				break;
			case ')':
				gotParen(ch);
				break;
			default:
				output = output + ch;
				break;
			}
		}

		while (!stack.isEmpty()) {
			System.out.println(stack);
			output = output + stack.pop();
		}
		System.out.println("End.\n" + stack);

		return output;
	}

	private void gotParen(char c) {
		while (!stack.isEmpty()) {
			char ch = stack.pop();
			if (ch == '(')
				break;
			else
				output += ch;
		}
	}

	private void gotOper(char opThis, int prec1) {
		while (!stack.isEmpty()) {
			char opTop = stack.pop();

			if (opTop == '(') {
				stack.push(opTop);
				break;
			} else {
				int prec2 = getPrecedence(opTop);
				if (prec2 < prec1) {
					stack.push(opTop);
					break;
				} else {
					output += opTop;
				}
			}
		}
		stack.push(opThis);
	}

	private int getPrecedence(char ch) {
		return map.get(ch);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String expression = "A*(B+C)-D/(E+F)";
		ConvertToPostfix con = new ConvertToPostfix(expression);
		System.out.println(con.doTrans());
	}
}

class CharStackPost {
	private int maxSize;
	private char[] stackArray;
	private int top;

	public CharStackPost(int s) {
		maxSize = s;
		stackArray = new char[maxSize];
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

	public int size() {
		return top+1;
	}

	public char peekN(int n) {
		return stackArray[n];
	}

	public String toString() {
		String s = "Stack (bottom->top):\n";
		for(int i = 0; i < size(); ++i)
			s += String.format("%c ", peekN(i));
		return s;
	}
}