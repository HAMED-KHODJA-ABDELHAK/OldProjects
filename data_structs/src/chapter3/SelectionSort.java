package chapter3;

class SelectArray {
	private long[] a;
	private int nElements;

	public SelectArray(int max) {
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

	public void selectSort() {
		int out, min;

		for (out = 0; out < nElements -1; out++) {
			min = out;

			for (int in = out+1; in < nElements; ++in)
				if(a[in] < a[min])
					min = in;
			swap(out, min);
		}
	}

	private void swap(int one, int two) {
		long temp = a[one];
		a[one] = a[two];
		a[two] = temp;
	}
}

public class SelectionSort {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final int SIZE = 50;
		SelectArray arr = new SelectArray(SIZE);

		for (int i = 0; i < 50; ++i)
			arr.insert((long) (Math.random() * 100));

		arr.display();
		arr.selectSort();
		arr.display();
	}

}
