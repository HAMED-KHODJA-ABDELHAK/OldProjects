package chapter7AdvancedSort;

public class Partition {
	private long[] array;
	private int nElems;

	public Partition(int max) {
		array = new long[max];
		nElems = 0;
	}

	public void insert(long l) {
		array[nElems++] = l;
	}

	public int size() {
		return nElems;
	}

	public void display() {
		for (int i = 0; i < nElems; ++i)
			System.out.printf("%d ", array[i]);
		System.out.println();
	}

	public int partitionIt(int left, int right, long pivot) {
		int leftPtr = left - 1, rightPtr = right + 1;

		while (true) {
			/* Find the bigger item. */
			while (leftPtr < right && array[++leftPtr] < pivot)
				;

			/* Find the smaller item. */
			while (rightPtr > left && array[--rightPtr] > pivot)
				;

			if (leftPtr >= rightPtr)
				break;
			else
				swap(leftPtr, rightPtr);
		}

		return leftPtr;
	}

	public void swap(int left, int right) {
		long temp = array[left];
		array[left] = array[right];
		array[right] = temp;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int MAX = 32;
		Partition p = new Partition(32);

		for (int i = 0; i < MAX; ++i) {
			p.insert((int) (Math.random() * MAX * 2));
		}

		p.display();

		p.partitionIt(0, MAX - 1, MAX);

		p.display();
	}

}
