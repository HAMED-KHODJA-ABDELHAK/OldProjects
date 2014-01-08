/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Project Headers */
#include "CUnit/Basic.h"
#include "mpi.h"
#include "lib_f.h"

/******************* Constants/Macros *********************/
#define TEMP_FILE 		"temp.txt"
#define VALS_SIZE		20

/******************* Type Definitions *********************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Suite initialization function run before each test.
 */
int suite_init(void) {
    srand(time(NULL));

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
void test_compare_equal(void) {
    int a = 9, b = 9;
    CU_ASSERT(lib_compare(&a, &b) == 0);
}

/*
 * Compare function, a less than b case.
 */
void test_compare_less(void) {
    int a = 7, b = 9;
    CU_ASSERT(lib_compare(&a, &b) < 0);
}

/*
 * Compare function, a more than b case.
 */
void test_compare_more(void) {
    int a = 20, b = 9;
    CU_ASSERT(lib_compare(&a, &b) > 0);
}


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
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
       (NULL == CU_add_test(sharedSuite, "Compare: Equal..............", test_compare_equal)) ||
       (NULL == CU_add_test(sharedSuite, "Compare: Less...............", test_compare_less)) ||
       (NULL == CU_add_test(sharedSuite, "Compare: Greater............", test_compare_more))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
