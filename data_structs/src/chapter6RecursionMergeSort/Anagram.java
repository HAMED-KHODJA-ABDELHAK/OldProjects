package chapter6RecursionMergeSort;

import java.io.IOException;
import java.util.Scanner;

public class Anagram {
	static int size;
	static int count;
	static char[] arrChar = new char[100];

	public static void main(String[] args) throws IOException {
		System.out.println("Enter a word: ");

		Scanner s = new Scanner(System.in);
		arrChar = s.next().toCharArray();
		size = arrChar.length;
		count = 0;
		displayWord();

		doAnagram(size);
	}

	public static void doAnagram(int newSize) {
		if (newSize == 1)
			return;

		for (int i=0; i < newSize; ++i) {
			doAnagram(newSize - 1);
			if (newSize == 2) {
				System.out.printf("%-5d)  ",count++);
				displayWord();
			}
			rotate(newSize);
		}
	}

	public static void displayWord() {
		for (int i = 0; i < size; ++i)
			System.out.print(arrChar[i]);

		System.out.println();
	}

	public static void rotate(int newSize) {
		int i, position = size - newSize;
		char temp = arrChar[position];

		for (i = position + 1; i < size; ++i)
			arrChar[i - 1] = arrChar[i];
		arrChar[i - 1] = temp;
	}
}
