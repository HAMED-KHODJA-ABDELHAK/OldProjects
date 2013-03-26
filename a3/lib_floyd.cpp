/**
 * Template C++ file.
 * Function with references to pointers: func(int *&ptr);
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
#define SEP         ":"

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
    for (int i = 0; i < size; ++i) {
        a[i] = new int[size];

        for (int j = 0; j < size; ++j) {
            if (j == i) {
                a[i][j] = 0;
            } else {
                a[i][j] = INF;
            }
        }

    }

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
 * Format: i j value.
 * Example: 1 2 4 means edge from 1 to 2 with cost 4.
 */
void Matrix::read(std::istream& in) {
    int i(0), j(0);

    if (size == 0)
        in >> size;

    while (in >> i && in >> j && in.good()) {
        in >> a[i][j];
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
                out << i << ": ";

            out << a[i][j] << " ";

            if (j == size-1)
                out << endl;
        }
    }
}

/*
 * Coordinates of 2d block are:
 * t_l_r = top left row.
 * t_l_c = top left column.
 * b_r_r = bottom right row.
 * b_r_c = bottom right column.
 *
 * String format example defines a 2x2 block from 0,0 to 1,1 with values 1,2,3,4.
 * 0 0 1 1 : 1 2 3 4
 */
void Matrix::str_init(std::string& line) {
    int t_l_r(0), t_l_c(0), b_r_r(0), b_r_c(0), mid = line.find(SEP, 0);

    /* Split line in two. */
    string control = line.substr(0, mid);
    string rem = line.substr(mid+1);

    /* Read control info. */
    std::stringstream ss1(control);
    ss1 >> t_l_r >> t_l_c >> b_r_r >> b_r_c;

    /* Overwrite the 2d block sent in this matrix. */
    std::stringstream ss2(rem);
    for (int i = t_l_r; i <= b_r_r; ++i) {
        for (int j = t_l_c; j <= b_r_c; ++j) {
            ss2 >> a[i][j];
        }
    }
}

/*
 * See above string format explanation. This function just makes the string.
 */
string Matrix::str_send(int t_l_r, int t_l_c, int b_r_r, int b_r_c) {
    std::stringstream line;

    /* Output control info, then append the entries from the matrix. */
    line << t_l_r << " " << t_l_c << " " << b_r_r << " " << b_r_c << SEP;

    for (int i = t_l_r; i <= b_r_r; ++i) {
        for (int j = t_l_c; j <= b_r_c; ++j) {
            line << a[i][j] << " ";
        }
    }

    /* Trim last space. */
    return line.str().substr(0, line.str().length()-1);
}

/*
 * Simple helper, the path matrix should start all values at INF.
 */
void floyd::init_path(Matrix& p) {
    for (int i = 0; i < p.size; ++i) {
        p.a[i][i] = INF;
    }
}

/****************** Static Functions **********************/


/****************** Global Functions **********************/




