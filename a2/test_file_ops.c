/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Project Headers */
#include "CUnit/Basic.h"
#include "mpi.h"
#include "array_ops.h"
#include "file_ops.h"

/******************* Constants/Macros *********************/
#define TEMP_FILE 		"temp.txt"
#define AR_SIZE			10

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/
static FILE *f;

/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Suite initialization function run before each test.
 */
int suite_init(void) {
	if ((f = fopen(TEMP_FILE, "w+")) == NULL)
		return -1;

	return 0;
}

/* The suite cleanup function.
 */
int suite_clean(void) {
	if (fclose(f) != 0)
		return -1;

	remove(TEMP_FILE);

	return 0;
}

/*
 * Test the read and write files. Here I've assumed that read and write
 * only work together else they fail.
 */
void test_read_write_file(void) {
	int gen[AR_SIZE], read[AR_SIZE];

	lib_generate_numbers(gen, AR_SIZE);
	lib_write_file(TEMP_FILE, gen, AR_SIZE);
	lib_read_file(TEMP_FILE, read, AR_SIZE);

	for (int i = 0; i < AR_SIZE; ++i) {
		CU_ASSERT(gen[i] == read[i]);
	}
}

/*
 * Test to exercise the array tracing function.
 */
void test_trace_array(void) {
	int buf_size = 200, id = 2, ar_size = 5, ar[] = {20, 44, 1, 99, 7};
	char buf[buf_size], tag[] = "TEST", expected[] = "TEST: I am 2. I have numbers: 20 44 1 99 7 \n";

	//lib_trace_array(buf, buf_size, tag, ar, ar_size, id);
	CU_ASSERT_STRING_EQUAL(buf, expected);
}

void test_log() {

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
	   (NULL == CU_add_test(sharedSuite, "Write/Read", test_read_write_file)) ||
	   (NULL == CU_add_test(sharedSuite, "Simple Log", test_log)) ||
	   (NULL == CU_add_test(sharedSuite, "Array Trace", test_trace_array))
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
