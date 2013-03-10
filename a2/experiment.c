/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"
#include "array_ops.h"
#include "file_ops.h"
#include "CUnit/Basic.h"


/******************* Constants/Macros *********************/

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Test Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
	subgroup_info_t info = {0,0,0,0, 3};
	int d = 1;

    lib_subgroup_info(d+1, &info);
    printf("Group, mem: %d, %d\n", info.group_num, info.member_num);

}
