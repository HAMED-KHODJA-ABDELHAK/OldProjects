package chapter3;

class Person implements Comparable<Person> {
	private String lastName;
	private String firstName;
	private int age;

	public Person(String last, String first, int age) {
		this.lastName = last;
		this.firstName = first;
		this.age = age;
	}

	@Override
	public String toString() {
		return String.format("%s, %s is %d,", lastName, firstName, age);
	}

	public String getLast() {
		return lastName;
	}

	public int compareTo(Person p) {
		return lastName.compareTo(p.lastName);
	}
}

class SortObjects {
	private Person[] a;
	private int nElems;

	public SortObjects(int max) {
		a = new Person[max];
		nElems = 0;
	}

	public void insert(Person p) {
		a[nElems++] = p;
	}

	public void display() {
		for (Person p: a)
			System.out.println(p);
		System.out.println();
	}

	public void insertionSort() {
		int in, out;

		for (out = 1; out < nElems; ++out) {
			Person temp = a[out];
			in = out;

			while (in > 0 && a[in-1].compareTo(temp) > 0) {
				a[in] = a[in-1];
				--in;
			}

			a[in] = temp;
		}
	}
}

public class ObjectSort {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		final int maxSize = 10;

		SortObjects s = new SortObjects(maxSize);

		s.insert(new Person("Stewart", "John", 44));
		s.insert(new Person("Rodriguez", "Bender", 5));
		s.insert(new Person("Philip", "Fry", 44));
		s.insert(new Person("Professor", "The", 44));
		s.insert(new Person("Turranga", "Leela", 44));
		s.insert(new Person("Wong", "Amy", 44));
		s.insert(new Person("Man", "Pig", 44));


		s.display();
		s.insertionSort();
		s.display();
	}

}
