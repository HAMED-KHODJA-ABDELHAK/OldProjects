//!! All elements of the file (enum_types, struct_types, functions) will be namespaced to file.
//!! --> i.e. if file is: network_service.c, prefix all functions with something like ns_init(void) {}, 
//!! typedefs like ns_order_t, enums like ns_values_t.
/*
 * Name of file
 *
 * - Notes on what this file does.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
//#include <ctype.h> 
//#include <limits.h> 
//#include <assert.h>

/* Project Headers */ 
#include "project_header1.h"

/****************************** Constants/Macros **************************************************/
//!! Macros always first then constants.
/* See printk.h for KERN_ lvls. This macro allows variable levels to be printed or disabled.
 * The level passed in must be equal or less than gbl_log_lvl. Any negative disables. */
#undef LOG
#define LOG(lvl, fmt, args...); \
    if(*((char *)(lvl)+1)-'0' <= gbl_log_lvl) \
        printk(lvl "idcc [%s] " fmt, PNAME, ##args);

/* Constants */
#define MAX_ORDERS	100

/****************************** Type Definitions **************************************************/
typedef enum 
{
  VAL1 = 66, VAL2, VAL3,
} values_e; 
 

typedef struct order_s
{
    u_long   order_id; /* Unique order number from db */
    u_short  quantity;
    u_short  type; /* Type corresponds to int from x file. */
    float    price;
    char     address[MAX_PROTO]; /* Protocol of connection. */
} order_t;

typedef struct order_db_s 
{
    u_long   num_orders;
    order_t  orders[MAX_ORDERS];
} order_db_t

/******************************* Prototypes *******************************************************/
//!! Limit use of prototypes, if you can position library/foundation functions at top of file then their
//!! invocations later won't require prototypes since compiler will have already read their definitions.
static int process_order(order_t ord);

/****************************** Static Data Definitions *******************************************/
//!! Anything that needs to be allocated statically goes here. Prefer static over dynamic memory
//!! allocation if possible.
static order_db_t order_db;

/****************************** Static Functions **************************************************/
//!! All functions should be declared static unless required in the global namespace like main. 
/*
 * This function takes an order, processes it and puts it into the db.
 */
static int process_order(order_t ord) 
{
    /* Do stuff with order, put in db then. */
}


/****************************** Global Functions **************************************************/
int main(int argc, char **argv) 
{
    printf("Hello.");
  
    return 0;
}

/****************************** End of File *******************************************************/
