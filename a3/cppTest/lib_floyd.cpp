/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
 */
/********************* Header Files ***********************/
/* C++ Headers */
#include <iostream> /* Input/output objects. */
#include <fstream> /* File operations. */
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
//#include <cstdlib>
//#include <cstddef>
//#include <cctype>
#include <cstring>
//#include <cstdio>
//#include <climits>
#include <cassert>

/* Project Headers */
#include "lib_floyd.hpp"

/******************* Constants/Macros *********************/


/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;
using floyd::Matrix;

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
/* Take a size, adjacency will be 2d square array.
 * Allocate memory on the heap, deallocate in destructor.
 */
Matrix::Matrix(int _size) : size(_size) {
	a = new int*[size];
	for (int i = 0; i < size; ++i)
		a[i] = new int[size];
}

/*
 * Copy constructor, copy a whole array.
 * After allocating memory, do a memcopy.
 */
Matrix::Matrix(const Matrix &o) : size(o.size) {
	a = new int*[size];
	for (int i = 0; i < size; ++i) {
		a[i] = new int[size];
	}

	memcpy((void *)a, (void *)o.a, size*size*sizeof(int));
}

Matrix::~Matrix() {
	for (int i = 0; i < size; ++i) {
		delete[] a[i];
	}
	delete[] a;
}

/*
 * Assignment operator, check not self assign else copy over.
 */
Matrix& Matrix::operator=(const Matrix& o) {
	if (this != &o) {
		/* Free old array. */
		for (int i = 0; i < size; ++i) {
			delete[] a[i];
		}
		delete[] a;

		/* Allocate new array based on other size. */
		size = o.size;
		a = new int*[size];
		for (int i = 0; i < size; ++i) {
			a[i] = new int[size];
		}

		memcpy((void *)a, (void *)o.a, size*size*sizeof(int));
	}

	return *this;
}

/*
 * Read values from istream into the array.
 */
void Matrix::read(std::istream& values) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			values >> a[i][j];
		}
	}
}

/*
 * Dump contents of matrix to stream.
 */
void Matrix::print(std::ostream& out) {
	out << "Matrix of size: " << size << endl;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (j == 0)
				cout << i << ": ";

			cout << a[i][j] << " ";

			if (j == size-1)
				cout << endl;
		}
	}
}

/****************** Static Functions **********************/


/****************** Global Functions **********************/




