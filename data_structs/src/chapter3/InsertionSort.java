package chapter3;

class InsertArray {
	private long a[];
	private int size;

	public InsertArray(int max) {
		a = new long[max];
		size = 0;
	}

	public void insert(long value) {
		a[size++] = value;
	}

	public void display() {
		for (int i = 0; i < size; ++i)
			System.out.print(a[i] + " ");
		System.out.println();
	}

	public void insertionSort() {
		int in, out;

		for (out = 1; out < size; ++out) {
			long temp = a[out];
			in = out;

			while (in > 0 && a[in-1] >= temp) {
				a[in] = a[in-1];
				--in;
			}

			a[in] = temp;
		}
	}
}

public class InsertionSort {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final int SIZE = 50;
		InsertArray arr = new InsertArray(SIZE);

		for (int i = 0; i < 50; ++i)
			arr.insert((long) (Math.random() * 100));

		arr.display();
		arr.insertionSort();
		arr.display();
	}
}
