package chapter7AdvancedSort;

public class QuickSort1 {
	private int[] array;
	private int num;

	public QuickSort1(int max) {
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
		if (right - left <= 0)
			return;
		else {
			int pivot = array[right];

			int partition = partitionIt(left, right, pivot);
			recQuickSort(left, partition-1);
			recQuickSort(partition+1, right);
		}
	}

	private int partitionIt(int left, int right, int pivot) {
		/* Left (after ++), right-1 (after --) */
		int leftPtr = left-1, rightPtr = right;

		while (true) {
			while (array[++leftPtr] < pivot)
				;

			while (rightPtr > 0 && array[--rightPtr] > pivot)
				;

			if (leftPtr >= rightPtr)
				break;
			else
				swap(leftPtr, rightPtr);
		}
		/* Restore pivot. */
		swap(leftPtr, right);

		return leftPtr;
	}

	public void swap(int left, int right) {
		int temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int MAX_ELEM = 64;
		final int MAX_VAL = 100;
		QuickSort1 q = new QuickSort1(MAX_ELEM);

		for (int i = 0; i < MAX_ELEM; ++i) {
			q.insert((int) (Math.random() * MAX_VAL));
		}

		q.display();
		q.quickSort();
		q.display();
	}

}
