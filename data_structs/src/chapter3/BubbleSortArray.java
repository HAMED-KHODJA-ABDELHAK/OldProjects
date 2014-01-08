package chapter3;

class ArrayBub {
	private long[] a;
	private int nElements;

	public ArrayBub(int max) {
		a = new long[max];
		nElements = 0;
	}

	public void insert(long value) {
		a[nElements++] = value;
	}

	public void display() {
		for (int i = 0; i < nElements; ++i)
			System.out.print(a[i] + " ");
		System.out.println();
	}

	public void bubbleSort() {
		int out, in;

		for (out = nElements-1; out > 1; out--)
			for(in = 0; in < out; ++in)
				if (a[in] > a[in+1])
					swap(in, in+1);
	}

	private void swap(int one, int two) {
		long temp = a[one];
		a[one] = a[two];
		a[two] = temp;
	}
}

public class BubbleSortArray {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final int SIZE = 50;
		ArrayBub arr = new ArrayBub(SIZE);

		for (int i = 0; i < 50; ++i)
			arr.insert((long) (Math.random() * 100));

		arr.display();
		arr.bubbleSort();
		arr.display();
	}

}
