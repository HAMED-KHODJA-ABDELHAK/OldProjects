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
//#include <cstdlib>
//#include <cstddef>
//#include <cctype>
//#include <cstring>
//#include <cstdio>
//#include <climits>
//#include <cassert>

/* Project Headers */
#include <CUnit/Basic.h>
#include "lib_floyd.hpp"

/******************* Constants/Macros *********************/


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
 * Suite initialization function run before each test.
 */
int suite_init(void) {

    return 0;
}

/* The suite cleanup function.
 */
int suite_clean(void) {

    return 0;
}

/*
 * Compare function, equal case.
 */
void test_compare_empty(void) {
    int a = 9, b = 9;
    CU_ASSERT(a == b);
}

/*
 * Test matrix init string for making a matrix.
 */
void test_matrix_init_string(void) {
    int size = 3;
    int expect[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };
    string init("0 0 2 2:1 2 3 4 5 6 7 8 9");

    floyd::Matrix m(size);
    m.str_init(init);

    CU_ASSERT(m.size == size);
    for (int i = 0; i < m.size; ++i) {
        for (int j = 0; j < m.size; ++j) {
            CU_ASSERT(m.a[i][j] == expect[i][j]);
        }
    }
}

/*
 * Use init function above to initialize a matrix.
 * Test the compressed string returned for transmitting.
 */
void test_matrix_send_string(void) {
    int size = 3;
    string expect("0 0 2 2:1 2 3 4 5 6 7 8 9");

    floyd::Matrix m(size);
    m.str_init(expect);
    string line = m.str_send(0, 0, 2, 2);

    CU_ASSERT(line.compare(expect) == 0);
}

/*
 * Test power function when exponent is even.
 */
void test_power_even(void) {
    int base = 2, exp = 10, expected = 1024, actual;
    actual = floyd::lib_power(base, exp);

    CU_ASSERT(actual == expected);
}

/*
 * Test power function when exponent is odd.
 */
void test_power_odd(void) {
    int base = 2, exp = 7, expected = 128;
    int actual = floyd::lib_power(base, exp);

    CU_ASSERT(actual == expected);
}

/*
 * Test simple square root function, value has even square root.
 */
void test_sqrt_evenly(void) {
    int val(16), expect(4);

    int actual = floyd::lib_sqrt(val);

    CU_ASSERT(actual == expect);
}

/*
 * Test simple square root function, value has no even square root.
 */
void test_sqrt_zero(void) {
    int val(17), expect(0);

    int actual = floyd::lib_sqrt(val);

    CU_ASSERT(actual == expect);
}

/**
 * Main loop of the function.
 */
int main(void) {
       CU_pSuite sharedSuite = NULL;

       /* Initialize the CUnit test registry. */
       if (CUE_SUCCESS != CU_initialize_registry())
          return CU_get_error();

       /* Add a suite to the registry. */
       sharedSuite = CU_add_suite("Shared Suite", suite_init, suite_clean);
       if (NULL == sharedSuite) {
          CU_cleanup_registry();
          return CU_get_error();
       }

       /* Add the tests to the suite. */
       if (
           (NULL == CU_add_test(sharedSuite, "Matrix: Init String.........", test_matrix_init_string)) ||
           (NULL == CU_add_test(sharedSuite, "Matrix: Send String.........", test_matrix_send_string)) ||
           (NULL == CU_add_test(sharedSuite, "Power Function, Even........", test_power_even)) ||
           (NULL == CU_add_test(sharedSuite, "Power Function, Odd.........", test_power_odd))
          )
       {
          CU_cleanup_registry();
          return CU_get_error();
       }

       /* Run all tests using the CUnit Basic interface */
//       CU_basic_set_mode(CU_BRM_VERBOSE);
       CU_basic_run_tests();
       CU_cleanup_registry();
       return CU_get_error();}

