/*----------------------------------------------------------
Argument Handler

Parses through arguments and returns an array of valid
"keys" and "values"
----------------------------------------------------------*/
#include "argument_handler.h"

/*----------------------------------------------------------
Creates the argument list. Allocates memory, must call
free_args() 
----------------------------------------------------------*/
Args** args_make(int argc, char** argv){
	//allocate memory on the heap for the argument list
	Args** args = malloc(sizeof(Args*) * argc);
	for (int i = 0; i < argc; i++){
		args[i] = malloc(sizeof(Args));
		args[i]->key = NULL;
		args[i]->value = NULL;
	}

	return args;
}

/*----------------------------------------------------------
Frees an argument list
----------------------------------------------------------*/
void args_free(Args** args, int size){
	for (int i = 0; i < size; i++){
		free(args[i]->key);
		free(args[i]->value);
		free(args[i]);
	}
	free(args);
}

/*----------------------------------------------------------
Returns the key of an argument
----------------------------------------------------------*/
char* args_key(Args* arg){

}

/*----------------------------------------------------------
Returns the value of an argument
----------------------------------------------------------*/
char* args_value(Args* arg){

}

/*----------------------------------------------------------
Returns the value of the (nth) key.
ex. -f=first -f=second
args_key_value(args, "f", 1) will return "first"
args_key_value(args, "f", 2) will return "second"
will return NULL if n is outside bounds
----------------------------------------------------------*/
char* args_key_value(Args** args, char* key, int n){

}