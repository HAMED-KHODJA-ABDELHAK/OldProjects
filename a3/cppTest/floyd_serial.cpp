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
/**
 * Main loop of the function.
 */
int main(void) {
	int num;
	cout << "Floyd serial algorithm." << endl;

	std::ifstream fin("input.txt", std::ifstream::in);
	fin >> num;
	Matrix m(num);

	try {
		m.read(fin);
	} catch (std::exception& e) {
		cout << "Exception occurred reading input.txt, check format complies." << endl;
		return 1;
	}
	m.print(cout);
	int **a = m.get_array();
	cout << "Size: " << m.get_size() << endl << "First element: " << a[3][2] << endl;

	return 0;
}

