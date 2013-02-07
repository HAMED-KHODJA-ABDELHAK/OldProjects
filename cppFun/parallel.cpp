/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
//#include <fstream> /* File operations. */
//#include <sstream> /* String stream. */
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
#include <ctime>
//#include <cstddef>
//#include <cctype>
//#include <cstring>
//#include <cstdio>
//#include <climits>
//#include <cassert>

/* Project Headers */
#include "mpi.h"

/******************* Constants/Macros *********************/
#define DEF_DARTS 	5000000
#define RADIUS 		1.0
#define REAL_PI		3.14159265358979
#define MASTER		0

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/
/* For enums: Try to namesapce the common elements.
 * typedef enum {
 *	VAL_,
 * } name_e;
 */

/* For structs:
 * typedef struct name_s {
 *	int index;
 * } name_t;
 */

/**************** Static Data Definitions *****************/


/****************** Class Definitions *********************/


/****************** Static Functions **********************/


/****************** Global Functions **********************/
/*
 * Determines if a point is inside a circle of radius centered on origin.
 * X and Y are assumed to come in as numbers between 0 and 1 from rand.
 * If the distance from center is less than OR equal to radius, we say it is inside.
 */
int in_circle(double x, double y, double r) {
    double cent_x = (2*r*x) - r, cent_y = (2*r*y) - r;
    double dist = (cent_x * cent_x) + (cent_y * cent_y);

    return dist < r*r;
}

/*
 * Function goes through a number of darts, each time randomly gets a pair of x,y coords that
 * are inside a 1x1 square. Checks if point is in circle, if so increments cnt.
 */
int throw_darts(unsigned int darts) {
	double x, y;
	int cnt = 0;

    for (unsigned int i = 0; i < darts; ++i) {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;

        if (in_circle(x, y, RADIUS))
            cnt++;
    }

	return cnt;
}

/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
	int rank, size, darts, hits, all_hits, darts_per_task;
	double start;

	MPI::Init(argc, argv);
	start = MPI::Wtime();
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	srand(time(NULL));

	if (argc < 2)
		darts = DEF_DARTS;
	else
		darts = atoi(*++argv);
	darts_per_task = darts/size;

	hits = throw_darts(darts_per_task);

//	MPI::Intracomm::Reduce(hits, all_hits, 1,
//		MPI::Datatype::MPI_INT,
	MPI::COMM_WORLD.Reduce(&hits, &all_hits, 1, MPI::INT, MPI::SUM, 0);

	if (rank == MASTER) {
		cout << "The hit count was " << all_hits << " of " << darts << " with a final value of PI: "
				<< 4.0 * all_hits/darts << endl;
		cout << "Time taken by program " << MPI::Wtime() - start
				<< "seconds." << endl;
	}

	MPI::Finalize();

	return 0;
}

