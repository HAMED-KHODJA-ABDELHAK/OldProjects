/**
 *
 */
package chapter5LinkList;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * @author starcraftman
 *
 */
public class DoubleLinkListTest {
	private DoubleLinkList list;
	private final static String PROPER_INIT_LIST = "10 20 30 ";

	/**
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception {
		list = new DoubleLinkList();
	}

	/**
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception {
		list = null;
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#isEmpty()}.
	 */
	@Test
	public void testIsEmpty() {
		/* Test that new list is empty. */
		assertTrue(list.isEmpty());

		list.insertLast(10);

		assertFalse(list.isEmpty());
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#insertFirst(long)}.
	 */
	@Test
	public void testInsertFirst() {
		list.insertFirst(10);
		list.insertFirst(20);
		list.insertFirst(30);

		assertEquals("The insertions failed.", "30 20 10 ", list.toString());
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#insertLast(long)}.
	 */
	@Test
	public void testInsertLast() {
		list.insertLast(10);
		list.insertLast(20);
		list.insertLast(30);

		assertEquals("The insertions failed.", PROPER_INIT_LIST, list.toString());	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#deleteFirst()}.
	 */
	@Test
	public void testDeleteFirst() {
		list.insertLast(10);
		list.insertLast(20);
		list.insertLast(30);

		assertEquals("List couldn't initialize correctly", PROPER_INIT_LIST, list.toString());

		list.deleteFirst();
		assertEquals("Delete failed", "20 30 ", list.toString());
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#deleteLast()}.
	 */
	@Test
	public void testDeleteLast() {
		list.insertLast(10);
		list.insertLast(20);
		list.insertLast(30);

		assertEquals("List couldn't initialize correctly", PROPER_INIT_LIST, list.toString());

		list.deleteLast();
		assertEquals("Delete failed", "10 20 ", list.toString());
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#insertAfter()}.
	 */
	@Test
	public void testInsertAfter() {
		list.insertLast(10);
		list.insertAfter(10, 20);
		assertEquals("Insert After 1 link failed.", "10 20 ", list.toString());

		list.insertLast(30);

		assertEquals("List couldn't initialize correctly", PROPER_INIT_LIST, list.toString());

		list.insertAfter(20, 40);
		assertEquals("Insert after full list failed", "10 20 40 30 ", list.toString());
	}

	/**
	 * Test method for {@link chapter5LinkList.DoubleLinkList#deleteKey(long)}.
	 */
	@Test
	public void testDeleteKey() {
		list.insertLast(10);
		list.insertLast(20);
		list.insertLast(30);
		assertEquals("List couldn't initialize correctly", PROPER_INIT_LIST, list.toString());


		list.deleteKey(20);
		list.deleteKey(30);
		assertEquals("Delete key failed", "10 ", list.toString());

		list.deleteKey(10);

		assertTrue(list.isEmpty());
		assertEquals("List isn't empty.", "", list.toString());
	}
}
