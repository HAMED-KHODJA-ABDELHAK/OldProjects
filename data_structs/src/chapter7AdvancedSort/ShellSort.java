package chapter7AdvancedSort;

public class ShellSort {
	private int[] array;
	private int numE;

	public ShellSort(int max) {
		array = new int[max];
		numE = 0;
	}

	public void insert(int i) {
		array[numE++] = i;
	}

	public void display() {
		System.out.print("A=");

		for (int i: array)
			System.out.printf("%d ", i);
		System.out.println();
	}

	public void shellSort() {
		int inner, outer, temp, h = 1;

		/* Determine initial spacing value via Knuth's formula. */
		while (h <= numE/3)
			h = h * 3 + 1;

		while (h > 0) {
			for (outer = h; outer < numE; ++outer) {
				temp = array[outer];
				inner = outer;

				while (inner > h-1 && array[inner-h] >= temp) {
					array[inner] = array[inner-h];
					inner -= h;
				}

				array[inner] = temp;
			}

			h = (h-1) / 3;
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int MAX = 2048;
		ShellSort s = new ShellSort(MAX);

		for (int i = 0; i < MAX; ++i)
			s.insert((int) (Math.random() * MAX));

		s.display();

		s.shellSort();

		System.out.println("Done.");
		s.display();
	}

}
