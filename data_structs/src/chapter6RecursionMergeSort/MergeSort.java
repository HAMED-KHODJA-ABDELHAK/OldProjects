package chapter6RecursionMergeSort;

public class MergeSort {
	private int[] array;
	private int numElements;

	public MergeSort(int maxElements) {
		array = new int[maxElements];
		numElements = 0;
	}

	public void insert(int value) {
		array[numElements++] = value;
	}

	public void display() {
		int count = 0;
		for (long l : array) {
			System.out.printf("%d ", l);

			if (++count == numElements)
				break;
		}
		System.out.println();
	}

	public void setElements(int[] array) {
		this.array = array;
		this.numElements = array.length;
	}

	public int[] getElements() {
		return this.array;
	}

	public void mergeSortStart() {
		int[] workspace = new int[numElements];
		recMergeSort(workspace, 0, numElements - 1);
	}

	public void recMergeSort(int[] workspace, int lowerBound, int upperBound) {
		// We have only 1 element, collapse up.
		if (lowerBound == upperBound)
			return;

		// More than 1.
		else {
			int mid = (lowerBound + upperBound) / 2;

			recMergeSort(workspace, lowerBound, mid);
			recMergeSort(workspace, mid + 1, upperBound);

			merge(workspace, lowerBound, mid + 1, upperBound);
		}
	}

	public void merge(int[] workspace, int lowPtr, int highPtr, int upperBound) {
		int i = 0, lowerBound = lowPtr, mid = highPtr - 1;
		int n = upperBound - lowerBound + 1;

		while (lowPtr <= mid && highPtr <= upperBound) {
			if (array[lowPtr] < array[highPtr])
				workspace[i++] = array[lowPtr++];
			else
				workspace[i++] = array[highPtr++];
		}

		while (lowPtr <= mid)
			workspace[i++] = array[lowPtr++];

		while (highPtr <= upperBound)
			workspace[i++] = array[highPtr++];

		for (i = 0; i < n; ++i)
			array[lowerBound + i] = workspace[i];
	}

	public static void main(String[] args) {
		MergeSort m = new MergeSort(100);

		m.insert(64);
		m.insert(21);
		m.insert(33);
		m.insert(70);
		m.insert(12);
		m.insert(85);
		m.insert(44);
		m.insert(3);
		m.insert(99);
		m.insert(0);
		m.insert(108);
		m.insert(36);

		m.display();

		m.mergeSortStart();

		m.display();
	}
}
