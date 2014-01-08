package chapter7AdvancedSort;

public class QuickSort3 {
	private int[] array;
	private int num;
	private static int recLevel = 0;
	private final static int INSERTION_CUTOFF = 10;

	public QuickSort3(int max) {
		array = new int[max];
		num = 0;
	}

	public void insert(int val) {
		array[num++] = val;
	}

	public void display() {
		for (int i: array)
			System.out.printf("%d ", i);
		System.out.println();
	}

	public void quickSort() {
		recQuickSort(0, num-1);
		// insertionSort(0, num-1);
	}

	public void recQuickSort(int left, int right) {
		/* If we have one or less in range, return. */
		int size = right-left+1;
		System.out.printf("At recLevel: %d\n", recLevel++);

		if (size < INSERTION_CUTOFF)
			insertionSort(left, right);
		else {
			int median = medianOf3(left, right);
			int partition = partitionIt(left, right, median);

			recQuickSort(left, partition-1);
			recQuickSort(partition+1, right);
		}
		recLevel--;
	}

	private int partitionIt(int left, int right, int pivot) {
		/* Left (after ++), right-1 (after --) */
		int leftPtr = left, rightPtr = right-1;

		while (true) {
			while (array[++leftPtr] < pivot)
				;

			while (array[--rightPtr] > pivot)
				;

			if (leftPtr >= rightPtr)
				break;
			else
				swap(leftPtr, rightPtr);
		}
		/* Restore pivot. */
		swap(leftPtr, right-1);

		return leftPtr;
	}

	public void swap(int left, int right) {
		int temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}

	private void insertionSort(int left, int right) {
		int in, out;

		for (out = left+1; out <= right; ++out) {
			int temp = array[out];
			in = out;

			while (in > left && array[in-1] >= temp) {
				array[in] = array[in-1];
				--in;
			}

			array[in] = temp;
		}
	}

	/**
	 * Sorts the left, right and pivot values.
	 * @param left
	 * @param right
	 * @return
	 */
	public int medianOf3(int left, int right) {
		int centre = (left + right) / 2;

		if (array[left] > array[centre])
			swap(left, centre);

		if (array[left] > array[right])
			swap(left, right);

		if (array[centre] > array[right])
			swap(centre, right);

		swap(centre, right-1); /* Put the pivot at the right. */
		return array[right-1];
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int MAX_ELEM = 4096;
		final int MAX_VAL = 1000;
		QuickSort3 q = new QuickSort3(MAX_ELEM);

		for (int i = 0; i < MAX_ELEM; ++i) {
			q.insert((int) (Math.random() * MAX_VAL));
		}

		q.display();
		q.quickSort();
		q.display();
	}

}
