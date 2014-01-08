package chapter6RecursionMergeSort;

import java.util.ArrayList;

/**
 * Select a bunch of weights that specifically add to a given target weight
 * to put in a bag.
 * @author starcraftman
 *
 */
public class ExcerciseKnapsack {
	static int[] a;
	static ArrayList<String> answers;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int targetWeight = 20;
		int[] array = {11, 8, 7, 6, 5, 3, 20, 17, 4};

		a = array;
		answers = new ArrayList<String>();

		ExcerciseKnapsack.pick(targetWeight, -1, "");

		for (String s: answers) {
			System.out.printf("%s fit in your bag.\n", s);
		}
	}

	/**
	 * Recursive selection function to solve the Knapsack problem. An array of elements should
	 * be set statically in a. Answers will be inserted into ArrayList<String> of answers.
	 *
	 * @param weightLeft The target weight.
	 * @param index Index we are at in array.
	 * @param selected Current list of selected items.
	 */
	public static void pick(int weightLeft, int index, String selected) {
		/* Current selected string is solution */
		if (weightLeft == 0) {
			answers.add(selected);
			return ;
		}

		/* Copy current selected */
		String original = selected;

		/* Iterate each index beyond this one and call recursively. */
		for (int i = index + 1; i < a.length; ++i) {
			/* Don't recurse if it is pointless. */
			if (weightLeft - a[i] < 0)
				continue;

			selected = original + a[i] + ", ";

			System.out.printf("Target left: %2d, Current selection : %s\n", weightLeft - a[i], selected);
			pick(weightLeft - a[i], i, selected);
		}
	}
}
