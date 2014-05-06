/*----------------------------------------------------------
Argument Handler

Parses through arguments and returns an array of valid
"keys" and "values"
----------------------------------------------------------*/
#include "argument_handler"

Arguments::Arguments(int argc, char** argv){
	number_of_arguments = argc -1;
	keys = new string* [number_of_arguments];
	values = new string* [number_of_arguments];
	for (int i = 0; i < number_of_arguments; i++){
		keys[i] = NULL;
		values[i] = NULL;
	}
}

Arguments::~Arguments(){
	delete[] keys;
	delete[] values;
}

string* Arguments::key(int index){
	return keys[index];
}

string* Arguments::value(int index){
	return values[index];
}

int Arguments::get_number_of_arguments(){
	return number_of_arguments;
}
