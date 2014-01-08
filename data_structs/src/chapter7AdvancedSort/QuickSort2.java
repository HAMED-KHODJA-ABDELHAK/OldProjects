package chapter7AdvancedSort;

public class QuickSort2 {
	private int[] array;
	private int num;
	private static int recLevel = 0;

	public QuickSort2(int max) {
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
	}

	public void recQuickSort(int left, int right) {
		/* If we have one or less in range, return. */
		int size = right-left+1;
		System.out.printf("At recLevel: %d\n", recLevel++);

		if (size < 4)
			manualSort(left, right);
		else {
			int median = medianOf3(left, right);

			int partition = partitionIt(left, right, median);
			recQuickSort(left, partition-1);
			recQuickSort(partition+1, right);
		}
		recLevel--;
	}

	private void manualSort(int left, int right) {
		int size = right - left + 1;

		if (size == 1)
			return;
		else if (size == 2) {
			// Check if one smaller than other.
			if (array[left] > array[right])
				swap(left, right);
			return;
		} else {
			// Size of 3.
			int centre = left + 1;

			if (array[left] > array[centre])
				swap(left, centre);

			if (array[left] > array[right])
				swap(left, right);

			if (array[centre] > array[right])
				swap(centre, right);		}
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
		final int MAX_ELEM = 64;
		final int MAX_VAL = 100;
		QuickSort2 q = new QuickSort2(MAX_ELEM);

		for (int i = 0; i < MAX_ELEM; ++i) {
			q.insert((int) (Math.random() * MAX_VAL));
		}

		q.display();
		q.quickSort();
		q.display();
	}

}
