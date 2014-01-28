//!! General Note: Minimize API exposed via top level files to prevent namespace pollution when including.
//!! -- In general that means keeping most of the code contained to the file statically.
//!! All includes have a guard at the top. Must match current name of file.
#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

/****************************** Header Files ******************************************************/
//!! Place all C library headers before any project specific ones that we write.
/* C Headers */

/* Project Headers */

/****************************** Constants/Macros **************************************************/
//!! Place constants here required to use the type definitions and functions publicly exposed.

/****************************** Type Definitions **************************************************/
//!! The end char of a type defines it, _e for any enum, _s for struct, and _t for typedefed structs.
/* For enums: Try to namesapce the common elements. 
 * typedef enum 
 * {
 *	VAL_,
 * } name_e; 
 */

/* For structs: 
 * typedef struct name_s 
 * {
 *	int index;
 * } name_t;  
 */

/****************************** Prototypes ********************************************************/
//!! Public API to be used by other parts of program go here.

#endif /* _EXAMPLE_H_ */
//!! Note comment, use to ensure you don't confuse yourself if you have mutliple IF directives.

