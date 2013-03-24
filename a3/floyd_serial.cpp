/**
 * Important notes on input.txt
 * First number on first line is number of nodes in graph.
 * All nodes on subsequent lines are zero indexed (i.e. node 1 -> 0).
 * Remaining lines must take following format:
 * i	j	cost
 * Reads from node i to node j the edge has cost to travel.
 * If you want undirected graph you'll put one i,j pair then j,i pair.
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
#include <sstream> /* String stream. */
#include <string> /* C++ String class. */
//#include <new> /* Defines bad_malloc exception, new functions. */
//#include <typeinfo> /* Casting header. */
//#include <exception> /* Top level exception header. */
//#include <stdexcept> /* Derived exception classes. */

/* STL Headers */
//#include <vector>
//#include <list>
//#include <deque>
//#include <stack>
//#include <queue>
//#include <priority_queue>
//#include <bitset>
//#include <set> // multiset for multiple keys allowed.
//#include <map> // multimap for multiple keys allowed.
//#include <utility> // Has pair for map.
//#include <algorithm>
//#include <numeric>

/* C Headers */
#include <cstdlib>
//#include <cstddef>
//#include <cctype>
//#include <cstring>
//#include <cstdio>
//#include <climits>
//#include <cassert>

/* Project Headers */
#include "lib_floyd.hpp"

/******************* Constants/Macros *********************/
#define OUTPUT		"output.txt"

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;
using floyd::Matrix;

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/
void serial_shortest(Matrix& cost, Matrix& path);
string make_path(const int i, const int j, const Matrix& dist, const Matrix& p);

/****************** Class Definitions *********************/


/****************** Static Functions **********************/


/****************** Global Functions **********************/
/**
 * Main loop of the function.
 */
int main(void) {
	int num;
	cout << "Floyd serial algorithm." << endl;

	std::ifstream fin("input.txt", std::ifstream::in);
	fin >> num;
	Matrix c(num); /* Cost matrix. */
	Matrix p(num); /* Path matrix. */

	floyd::init_path(p);
	p.print(cout);

	c.read(fin);
	cout << "The original cost matrix." << endl;
	c.print(cout);

	serial_shortest(c, p);

	cout << "The shortest path matrix." << endl;
	c.print(cout);

	cout << "Path from node 2 -> 4 has cost " << c.a[1][3] << " and is: 2 " << make_path(1, 3, c, p) << " 4 " << endl;
	p.print(cout);

	return 0;
}

void serial_shortest(Matrix& cost, Matrix& path) {
	for (int k = 0; k < cost.size; ++k) {
		for (int i = 0; i < cost.size; ++i) {
			for (int j = 0; j < cost.size; ++j) {
				int new_dist = cost.a[i][k] + cost.a[k][j];
				if (new_dist < cost.a[i][j]) {
					cost.a[i][j] = new_dist;
					path.a[i][j] = k;
				}
			}
		}
	}
}

/*
 * Get the path from node i to j (zero index) with distance and path matrices.
 */
string make_path(const int i, const int j, const Matrix& dist, const Matrix& p) {
	if (dist.a[i][j] == INF)
		return string("NO PATH.");

	int mid = p.a[i][j];

	if (mid == INF) {
		return string(" ");
	}
	std::stringstream line;
	line << make_path(i, mid, dist, p) << mid+1 << make_path(mid, j, dist, p);

	return line.str();
}
