package chapter6RecursionMergeSort;

public class Towers {
	static int nDisks = 3;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		doTowers(nDisks, 'A', 'B', 'C');
	}

	/**
	 * Towers of Hanoi algorithm.
	 * @param topN Number of disks remaining to move for this subtree.
	 * @param from From pile.
	 * @param inter Intermediate.
	 * @param to Destination.
	 */
	public static void doTowers(int topN, char from, char inter, char to) {
		if (topN == 1)
			System.out.printf("Disk 1 from %c to %c\n", from, to);
		else {
			doTowers(topN - 1, from, to, inter);

			System.out.printf("Disk %d from %c to %c\n", topN, from, to);

			doTowers(topN - 1, inter, from, to);
		}
	}
}
