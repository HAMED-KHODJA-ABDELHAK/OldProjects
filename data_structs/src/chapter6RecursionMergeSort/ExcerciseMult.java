package chapter6RecursionMergeSort;

public class ExcerciseMult {
	public static int mult(int x, int times) {
		if (times == 1)
			return x;

		int result = 0;
		if (times % 2 == 0) {
			result = mult(x, times/2);
			return result + result;
		} else { // Odd.
			result = mult(x, (times-1)/2);
			return x + result + result;
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println(mult(4, 2));

	}

}
