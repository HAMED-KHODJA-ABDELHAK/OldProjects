package chapter10Tree234;

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class Tree234Test {
	private static final String ORIGINAL = "/10/20/30/40/50/60/70/80/90";
	private Tree234 t;

	@Before
	public void setUp() throws Exception {
		t = new Tree234();

		t.insert(50);
		t.insert(40);
		t.insert(60);
		t.insert(30);
		t.insert(70);
		t.insert(10);
		t.insert(80);
		t.insert(20);
		t.insert(90);
	}

	@After
	public void tearDown() throws Exception {
		t = null;
	}

	@Test
	public void testFind() {
		assertEquals(0, t.find(30));
		assertEquals(-1, t.find(150));
	}

	@Test
	public void testInsert() {
		String soFar = ORIGINAL + "/100";
		t.insert(100);
		assertEquals(soFar, t.inOrderTraversal(t.getRoot(), ""));

		soFar = "/5" + soFar;
		t.insert(5);
		assertEquals(soFar, t.inOrderTraversal(t.getRoot(), ""));

		soFar = soFar.substring(0, soFar.indexOf("/60")) + "/55" +
				soFar.substring(soFar.indexOf("/60"));
		t.insert(55);
		assertEquals(soFar, t.inOrderTraversal(t.getRoot(), ""));
	}

	@Test
	public void testGetNextChild() {
		Node n = t.getNextChild(t.getRoot(), 90);
		assertEquals("/80/90", t.inOrderTraversal(n, ""));
	}

	@Test
	public void testGetMinimum() {
		assertEquals(90, t.getMaximum());
	}

	@Test
	public void testGetMaximum() {
		assertEquals(10, t.getMinimum());
	}

	@Test
	public void testInOrderTraversal() {
		assertEquals(ORIGINAL, t.inOrderTraversal(t.getRoot(), ""));
	}
}
