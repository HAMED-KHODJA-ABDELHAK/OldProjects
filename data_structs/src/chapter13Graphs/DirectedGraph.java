package chapter13Graphs;

import java.util.LinkedList;

public class DirectedGraph {
	private final int MAX_VERTS = 20;
	private Vertex vertexList[];
	private int adjMat[][];
	private int nVerts;
	private LinkedList<Character> list;

	public DirectedGraph() {
		vertexList = new Vertex[MAX_VERTS];

		adjMat = new int[MAX_VERTS][MAX_VERTS];
		nVerts = 0;
		for (int  i = 0; i < MAX_VERTS; ++i)
			for (int j = 0; j < MAX_VERTS; ++j)
				adjMat[i][j] = 0;
		list = new LinkedList<Character>();
	}

	public void addVertex(char label) {
		vertexList[nVerts++] = new Vertex(label);
	}

	public void addEdge(int start, int end) {
		adjMat[start][end] = 1;
	}

	public void displayVertex(int v) {
		System.out.print(vertexList[v]);
	}

	/**
	 * Checks for a vertex with no sucessors in the directed graph.
	 * @return The index of the next vertex or -1 if no others.
	 */
	public int noSucessors() {
		boolean isEdge;

		/* Iterate over every row/column in matrix. Verify a given vertex has no edges. */
		for (int row = 0; row < nVerts; ++row) {
			isEdge = false;

			for (int col = 0; col < nVerts; ++col) {
				if (adjMat[row][col] > 0) {
					isEdge = true;
					break;
				}
			}

			/* Return this vertex if no edges */
			if (!isEdge)
				return row;
		}

		/* No more edges remain. */
		return -1;
	}

	private void deleteVertex(int dVertex) {
		if (dVertex != nVerts -1) {
			for (int i = dVertex; i < nVerts; ++i)
				vertexList[i] = vertexList[i + 1];

			for (int row = dVertex; row < nVerts - 1; ++row)
				moveRowUp(row, nVerts);

			for (int col = dVertex; col < nVerts -1; ++col)
				moveColLeft(col, nVerts -1);
		}

		nVerts--;
	}

	private void moveRowUp(int row, int length) {
		for (int col = 0; col < length; ++col)
			adjMat[row][col] = adjMat[row + 1][col];
	}

	private void moveColLeft(int col, int height) {
		for (int row = 0; row < height; ++row)
			adjMat[row][col] = adjMat[row][col + 1];
	}

	public void topo() {
		while (nVerts > 0) {
			int currentVertex = noSucessors();

			if (currentVertex == -1) {
				System.out.println("Err: Graph has cycles.");
				return;
			}

			/* Insert the returned vertex in list. */
			list.addFirst(vertexList[currentVertex].getLabel());
			deleteVertex(currentVertex);
		}

		System.out.print("Topologically sorted order: ");
		for (Character c: list)
			System.out.print(c);
		System.out.println();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		DirectedGraph d = new DirectedGraph();

		d.addVertex('A');
		d.addVertex('B');
		d.addVertex('C');
		d.addVertex('D');
		d.addVertex('E');
		d.addVertex('F');
		d.addVertex('G');
		d.addVertex('H');

		d.addEdge(0, 3);
		d.addEdge(0, 4);
		d.addEdge(1, 4);
		d.addEdge(2, 5);
		d.addEdge(3, 6);
		d.addEdge(4, 6);
		d.addEdge(5, 7);
		d.addEdge(6, 7);

		d.topo();
	}

}
