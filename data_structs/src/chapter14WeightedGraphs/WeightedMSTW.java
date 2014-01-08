package chapter14WeightedGraphs;

class Edge {
	public int srcVert;
	public int dstVert;
	public int distance;

	public Edge(int sv, int dv, int d) {
		srcVert = sv;
		dstVert = dv;
		distance = d;
	}

	@Override
	public String toString() {
		return String.format("srcVert: %d, dstVert %d, distance %d.\n", srcVert, dstVert, distance);
	}
}

class PriorityQ {
	private final int SIZE = 20;
	private Edge[] queArray;
	private int size;

	public PriorityQ() {
		queArray = new Edge[SIZE];
		size = 0;
	}

	public void insert(Edge e) {
		int j;

		for (j = 0; j < size; ++j)
			if (e.distance >= queArray[j].distance)
				break;

		for (int k = size-1; k >= j; --k)
			queArray[k+1] = queArray[k];

		queArray[j] = e;
		size++;
	}

	public Edge removeMin() {
		return queArray[--size];
	}

	public void removeN(int n) {
		for (int i=n; i < size-1; ++i)
			queArray[i] = queArray[i+1];
		size--;
	}

	public Edge peekMin() {
		return queArray[size-1];
	}

	public int size() {
		return size;
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public Edge peekN(int n) {
		return queArray[n];
	}

	public int find(int destVert) {
		for (int i = 0; i < size; ++i)
			if (queArray[i].dstVert == destVert)
				return i;

		return -1;
	}
}

class Vertex {
	public char label;
	public boolean isInTree;

	public Vertex(char lab) {
		label = lab;
		isInTree = false;
	}

	@Override
	public String toString() {
		return String.format("Label %c, isInTree %s\n", label, isInTree ? "True" : "False");
	}
}

class Graph {
	private final int MAX_VERTS = 20;
	private final int INFINITY = 1000000;
	private Vertex vertexList[];
	private int adjMat[][];
	private int nVerts;
	private int currentVert;
	private PriorityQ thePQ;
	private int nTree;

	public Graph() {
		vertexList = new Vertex[MAX_VERTS];
		adjMat = new int[MAX_VERTS][MAX_VERTS];
		nVerts = 0;

		for (int i = 0; i < MAX_VERTS; ++i)
			for (int j = 0; j < MAX_VERTS; ++j)
				adjMat[i][j] = INFINITY;

		thePQ = new PriorityQ();
	}

	public void addVertex(char lab) {
		vertexList[nVerts++] = new Vertex(lab);
	}

	public void addEdge(int start, int end, int weight) {
		adjMat[start][end] = weight;
		adjMat[end][start] = weight;
	}

	public void displayVertex(int v) {
		System.out.print(vertexList[v].label);
	}

	public void putInPQ(int newVert, int newDist) {
		int queueIndex = thePQ.find(newVert);

		if (queueIndex != -1) { /* Already exists, check if newDist is less than old. */
			Edge tempEdge = thePQ.peekN(queueIndex);
			int oldDist = tempEdge.distance;
			if (oldDist > newDist) {
				thePQ.removeN(queueIndex);
				thePQ.insert(new Edge(currentVert, newVert, newDist));
			}
		} else { /* Not found, insert */
			thePQ.insert(new Edge(currentVert, newVert, newDist));
		}
	}

	/* Minimum spanning tree of a weighted graph. */
	public void mstw() {
		currentVert = 0;

		while (nTree < nVerts-1) {
			vertexList[currentVert].isInTree = true;
			nTree++;

			for (int i = 0; i < nVerts; ++i) {
				if (i == currentVert)
					continue;
				if (vertexList[i].isInTree)
					continue;

				int distance = adjMat[currentVert][i];
				if (distance == INFINITY)
					continue;

				putInPQ(i, distance);
			}

			if (thePQ.size() == 0) {
				System.out.println("Graph not connected");
				return;
			}

			Edge minEdge = thePQ.removeMin();
			int sourceVert = minEdge.srcVert;
			currentVert = minEdge.dstVert;

			System.out.print(vertexList[sourceVert].label);
			System.out.print(vertexList[currentVert].label);
			System.out.print(" ");
		}

		for (int i = 0; i < nVerts; ++i)
			vertexList[i].isInTree = false;
	}
}

public class WeightedMSTW {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Graph g = new Graph();
		g.addVertex('A'); // Chars map A-F onto 1-6
		g.addVertex('B');
		g.addVertex('C');
		g.addVertex('D');
		g.addVertex('E');
		g.addVertex('F');

		g.addEdge(0, 1, 6);
		g.addEdge(0, 3, 4);
		g.addEdge(1, 2, 10);
		g.addEdge(1, 3, 7);
		g.addEdge(1, 4, 7);
		g.addEdge(2, 3, 8);
		g.addEdge(2, 4, 5);
		g.addEdge(2, 5, 6);
		g.addEdge(3, 4, 12);
		g.addEdge(4, 5, 7);

		System.out.println("The minimum spanning tree:");
		g.mstw();
		System.out.println();
	}
}
