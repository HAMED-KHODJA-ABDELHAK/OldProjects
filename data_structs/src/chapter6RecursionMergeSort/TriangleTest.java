package chapter6RecursionMergeSort;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class TriangleTest {

	@Before
	public void setUp() throws Exception {
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testTriangle() {
		int number = 10;
		assertEquals(55, Triangle.triangle(number));
	}

}
