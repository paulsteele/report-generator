#ifndef _ARG_
#define _ARG_
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------
Abstract structure for the arguments.
ex. -foo=bar
if not in this format, then key && value will be NULL
----------------------------------------------------------*/
typedef struct args {
	char* key; //ex. key would be "foo" 
	char* value; //ex. value would be "bar"
} Args;

/*----------------------------------------------------------
Creates the argument list. Allocates memory, must call
free_args() 
----------------------------------------------------------*/
Args** args_make(int argc, char** argv);

/*----------------------------------------------------------
Frees an argument list
----------------------------------------------------------*/
void args_free(Args** arg, int size);

/*----------------------------------------------------------
Returns the key of an argument
----------------------------------------------------------*/
char* args_key(Args* arg);

/*----------------------------------------------------------
Returns the value of an argument
----------------------------------------------------------*/
char* args_value(Args* arg);

/*----------------------------------------------------------
Returns the value of the (nth) key.
ex. -f=first -f=second
args_key_value(args, "f", 1) will return "first"
args_key_value(args, "f", 2) will return "second"
will return NULL if n is outside bounds
----------------------------------------------------------*/
char* args_key_value(Args** args, char* key, int n);

#endif