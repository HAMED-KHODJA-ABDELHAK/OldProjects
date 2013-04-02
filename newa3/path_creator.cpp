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

/* C Headers */
#include <cstdlib>
#include <cstring>

/* Project Headers */

/******************* Constants/Macros *********************/
#define SEP				":"
#define INF				10000

/**************** Namespace Declarations ******************/
using std::cin;
using std::cout;
using std::endl;
using std::string;

/******************* Type Definitions *********************/


/****************** Class Definitions *********************/
/*
 * Public fields reduce this to mostly a struct.
 */
class Matrix {
public:
	Matrix() : size(0), a(NULL) { }
	Matrix(int _size);
	Matrix(const Matrix& m);
	virtual ~Matrix();
	Matrix& operator=(const Matrix& o);

	/* Functions */
	void read(std::istream& in);
	void print(std::ostream& out);
	void str_init(std::string& line);
	std::string str_send(int t_l_r, int t_l_c, int b_r_r, int b_r_c);

	/* Public vars by design, not good oop. */
	int size;
	int **a;
};

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
void init_path(Matrix& p) {
    for (int i = 0; i < p.size; ++i) {
        p.a[i][i] = INF;
    }
}

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/****************** Global Functions **********************/
/*
 * Get the path from node i to j (zero index) with distance and path matrices.
 * You need to put i and j around what is returned from this function.
 */
string make_path(const int i, const int j, const Matrix& dist, const Matrix& p) {
	if (dist.a[i][j] == INF)
		return string("NO PATH.");

	int mid = p.a[i][j];

	if (mid == INF) {
		return string(" ");
	}

	std::stringstream line;
	line << make_path(i, mid, dist, p) << mid << make_path(mid, j, dist, p);

	return line.str();
}

/**
 * Main loop of the function.
 */
int main(void) {
	int i = 0, j = 0, nodes = 0;

	/* Open input and make matrices. */
	std::ifstream cost_file("cost.txt", std::ifstream::in);
	std::ifstream path_file("path.txt", std::ifstream::in);
	cost_file >> nodes;
	path_file >> nodes;

	Matrix c(nodes); /* Cost matrix. */
	Matrix p(nodes); /* Path matrix. */
	init_path(p);

	/* Expect cost matrix to be in current dir, generated by other programs. */
	c.read(cost_file);
	p.read(path_file);

	/* Query interface, query about any shortest path to get nodes. Zero indexed as always. */
	while (true) {
		cout << "Enter an i and j and I will tell you the shortest path and cost." << endl <<
				"Enter -1 on either to quit." << endl;

		/* Check bad input. */
		cin >> i >> j;
		if (!cin) {
			cout << "Bad input, only integers allowed. Please try again." << endl;
			cin.clear();
			std::string temp;
			cin >> temp;
			continue;
		}

		if (-1 == i || -1 == j)
			break;

		if (i > c.size || j > c.size || i < 0 || j < 0) {
			cout << "Numbers out of range. Only 0 - " << c.size-1 << " allowed." << endl;
			continue;
		}

		cout << "The path from path is: " << i << " " << make_path(i, j, c, p) << " " << j << " " << endl
				<< "The cost of the path is: " << c.a[i][j] << endl;
	}

	return 0;
}
