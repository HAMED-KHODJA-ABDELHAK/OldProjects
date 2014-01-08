package chapter13Graphs;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Stack;

public class GraphWithList {
	private final int MAX_VERTS = 20;
	private Vertex vertexList[];
	private ArrayList<LinkedList<Integer>> adjList;
	private int nVerts;
	private Stack<Integer> stack;
	private LinkedList<Integer> queue;

	public GraphWithList() {
		vertexList = new Vertex[MAX_VERTS];
		adjList = new ArrayList< LinkedList<Integer> >();
		nVerts = 0;
		stack = new Stack<Integer>();
		queue = new LinkedList<Integer>();
	}

	public void addVertex(char label) {
		vertexList[nVerts++] = new Vertex(label);
	}

	public void addEdge(int start, int end) {
		adjList.get(start).add(end);
		adjList.get(end).add(start);
	}

	public void displayVertex(int vertexNum) {
		System.out.print(vertexList[vertexNum].label);
	}

	/**
	 * Execute a minimum spanning tree algorithm.
	 */
	public void mst() {
		vertexList[0].wasVisited = true;
		stack.push(0);

		while (!stack.isEmpty()) {
			int currentVertex = stack.peek();
			int v = getAdjUnvisitedVertex(currentVertex);

			if (v == -1)
				stack.pop();
			else {
				vertexList[v].wasVisited = true;
				stack.push(v);

				displayVertex(currentVertex);
				displayVertex(v);
				System.out.print(" ");
			}
		}

		for (int i = 0; i < nVerts; ++i)
			vertexList[i].wasVisited = false;
	}

	public void dfs() {
		vertexList[0].wasVisited = true;
		displayVertex(0);
		stack.push(0);

		while (!stack.isEmpty()) {
			int v = getAdjUnvisitedVertex(stack.peek());

			if (v == -1)
				stack.pop();
			else {
				vertexList[v].wasVisited = true;
				displayVertex(v);
				stack.push(v);
			}
		}

		for (int i = 0; i < nVerts; ++i)
			vertexList[i].wasVisited = false;
	}

	public void bfs() {
		vertexList[0].wasVisited = true;
		displayVertex(0);
		queue.push(0);
		int v2;

		while (!queue.isEmpty()) {
			int v1 = queue.remove();

			while ( (v2 = getAdjUnvisitedVertex(v1)) != -1) {
				vertexList[v2].wasVisited = true;
				displayVertex(v2);
				queue.push(v2);
			}
		}

		for (int i = 0; i < nVerts; ++i)
			vertexList[i].wasVisited = false;
	}

	/**
	 * Returns index of first unvisited node adjacent to node index v.
	 * @param v Node to examine for adjacent.
	 * @return Index of an adjacent node, else -1.
	 */
	public int getAdjUnvisitedVertex(int v) {
		LinkedList<Integer> l = adjList.get(v);
		for (Integer i : l)
			if (vertexList[i].wasVisited == false)
				return i;

		return -1;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Graph g = new Graph();

		g.addVertex('A');
		g.addVertex('B');
		g.addVertex('C');
		g.addVertex('D');
		g.addVertex('E');

		g.addEdge(0, 1);
		g.addEdge(0, 2);
		g.addEdge(0, 3);
		g.addEdge(0, 4);
		g.addEdge(1, 2);
		g.addEdge(1, 3);
		g.addEdge(1, 4);
		g.addEdge(2, 3);
		g.addEdge(2, 4);
		g.addEdge(3, 4);

		System.out.print("Visits: ");
		g.bfs();
		System.out.println();
	}
}