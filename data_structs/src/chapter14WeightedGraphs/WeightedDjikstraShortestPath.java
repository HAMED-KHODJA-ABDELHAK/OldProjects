package chapter14WeightedGraphs;

class DistPar {
	public int distance;
	public int parentVert;

	public DistPar(int dist, int par) {
		distance = dist;
		parentVert = par;
	}

	@Override
	public String toString() {
		return String.format("distance %d, parentVert %d.\n", distance, parentVert);
	}
}

/* Vertex defined in WeightedMSTW. */

class DJGraph {
	private final int MAX_VERTS = 20;
	private final int INFINITY = 1000000;
	private Vertex vertexList[];
	private DistPar sPath[]; /* Shortest path Array */
	private int adjMat[][];
	private int nVerts;
	private int nTree;
	private int currentVert;
	private int startToCurrent; /* Distance to current vert */

	public DJGraph() {
		vertexList = new Vertex[MAX_VERTS];
		adjMat = new int[MAX_VERTS][MAX_VERTS];
		sPath = new DistPar[MAX_VERTS];
		nVerts = 0;
		nTree = 0;

		for (int i = 0; i < MAX_VERTS; ++i)
			for (int j = 0; j < MAX_VERTS; ++j)
				adjMat[i][j] = INFINITY;
	}

	public void addVertex(char lab) {
		vertexList[nVerts++] = new Vertex(lab);
	}

	public void addEdge(int start, int end, int weight) {
		adjMat[start][end] = weight;
	}

	public int getMin() {
		int minDist = INFINITY;
		int minIndex = 0;
		for (int i = 0; i < nVerts; ++i)
			if (vertexList[i].isInTree == false && sPath[i].distance < minDist) {
				minDist = sPath[i].distance;
				minIndex = i;
			}

		return minIndex;
	}

	public void adjustShortestPath() {
		int column = 1;
		while (column < nVerts) {
			if (vertexList[column].isInTree) {
				++column;
				continue;
			}

			/* Calculate distance for one path entry */
			int currentToFringe = adjMat[currentVert][column];
			int startToFringe = startToCurrent + currentToFringe;
			int sPathDist = sPath[column].distance;

			/* Update as needed. */
			if (startToFringe < sPathDist) {
				sPath[column].parentVert = currentVert;
				sPath[column].distance = startToFringe;
			}

			++column;
		}
	}

	public void path() {
		int startTree = 0;
		vertexList[startTree].isInTree = true;
		nTree = 1;

		/* Take first line as shortest first path. */
		for (int i = 0; i < nVerts; ++i)
			sPath[i] = new DistPar(adjMat[startTree][i], startTree);

		while (nTree <  nVerts) {
			int minIndex = getMin();
			int minDist = sPath[minIndex].distance;

			if (minDist == INFINITY) {
				System.out.println("There are unreachable vertices.");
				break;
			}
			else {
				currentVert = minIndex;
				startToCurrent = sPath[minIndex].distance;
			}

			vertexList[currentVert].isInTree = true;
			nTree++;
			adjustShortestPath();
		}

		displayPaths();
	}

	private void displayPaths() {
		for (int i = 0; i < nVerts; ++i) {
			System.out.print(vertexList[i].label + "=");

			if (sPath[i].distance == INFINITY)
				System.out.print("inf");
			else
				System.out.print(sPath[i].distance);

			char parent = vertexList[sPath[i].parentVert].label;
			System.out.printf("(%c) ", parent);
		}

		System.out.println();
	}
}

public class WeightedDjikstraShortestPath {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		DJGraph g = new DJGraph();
		g.addVertex('A'); // Chars map A-F onto 1-6
		g.addVertex('B');
		g.addVertex('C');
		g.addVertex('D');
		g.addVertex('E');

		g.addEdge(0, 1, 50);
		g.addEdge(0, 3, 80);
		g.addEdge(1, 2, 60);
		g.addEdge(1, 3, 90);
		g.addEdge(2, 4, 40);
		g.addEdge(3, 2, 20);
		g.addEdge(3, 4, 70);
		g.addEdge(4, 1, 50);

		g.path();
	}

}
