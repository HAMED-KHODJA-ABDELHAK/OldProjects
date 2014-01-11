package chapter6RecursionMergeSort;

import java.util.Scanner;

public class Triangle {
	static int number;

	public static void main(String[] args) {
		System.out.println("Enter a number: ");
		Scanner s = new Scanner(System.in);
		number = s.nextInt();
		s.close();

		int answer = triangle(number);
		System.out.println("The answer: " + answer);
	}

	public static int triangle(int n) {
		if (n == 1)
			return 1;
		else
			return n + triangle(n-1);
	}
}
