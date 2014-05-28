/*----------------------------------------------------------
Argument Handler

Handles arguments passed into program.
Arguments have format "-KEY=VALUE". Value is optional 
----------------------------------------------------------*/
#include "argument_handler"

/*----------------------------------------------------------
	Constructs the Argument class
	argc = number of arguments to look at
	argv = array of char* of the arguments
----------------------------------------------------------*/
Arguments::Arguments(int argc, char** argv){
	//account for the program name being the first argument
	argv++; 
	number_of_arguments = argc - 1; 

	//allocate space for the keys and values
	keys = new string* [number_of_arguments];
	values = new string* [number_of_arguments];

	//fill up the keys and values
	for (int i = 0; i < number_of_arguments; i++){
		string* argument = new string(argv[i]); //take char* into string
		//set the keys and values
		string** formated = argument_to_keyvalue(argument);
		keys[i] = formated[0];
		values[i] = formated[1];
		//cleanup
		delete[] formated;
		delete argument;
	}
	//clean up, only holding values that mean something
	std::list<int> locations;
	for (int i = 0; i < argc - 1; i++){
		if (keys[i] == NULL){
			locations.push_back(i);
			--number_of_arguments;
		}
		else {
			if (!locations.empty()){
				int index = locations.front();
				keys[index] = keys[i];
				values[index] = values[i];
				locations.pop_front();
				locations.push_back(i);
				keys[i] = NULL;
				values[i] = NULL;
			}
		}
	}
}


/*----------------------------------------------------------
Frees data used in construction of argument list
----------------------------------------------------------*/
Arguments::~Arguments(){
	//Free all data held in keys and values
	for (int i = 0; i < number_of_arguments; i++){
		delete keys[i];
		delete values[i];
	}
	delete[] keys;
	delete[] values;
}

/*----------------------------------------------------------
Takes in a string as an argument. The argument should
be in format -KEY=VALUE value is optional. Will always return
a string array of size 2 but those strings may be NULL
----------------------------------------------------------*/
string** Arguments::argument_to_keyvalue(string* argument){
	string** keyvalue = new string* [2];
	//valid arguments have - as the first character
	if (argument->at(0) != '-'){
		keyvalue[0] = NULL;
		keyvalue[1] = NULL;
	}
	else {
		int seperator = argument->find_first_of("=", 1);
		//set the key as the substring up to the = (or the end)
		keyvalue[0] = new string(argument->substr(1, seperator -1));
		//determines if a valid value is behind the = sign,
		if (seperator == string::npos || seperator == argument->size() -1){
			keyvalue[1] = NULL;
			seperator = argument->size();
		}
		else{
			keyvalue[1] = new string(argument->substr(seperator+1));
		}
	}

	return keyvalue;
}

/*----------------------------------------------------------
Returns the key associated with the index number. NULL
is returned if outside bounds
----------------------------------------------------------*/
string* Arguments::key(int index){
	if (index < number_of_arguments)
		return keys[index];
	else
		return NULL;
}

/*----------------------------------------------------------
Returns the value associated with the index number. NULL
is returned if outside bounds
----------------------------------------------------------*/
string* Arguments::value(int index){
	if (index < number_of_arguments)
		return values[index];
	else
		return NULL;
}

/*----------------------------------------------------------
Returns the value associated with the key. NULL if string
not found. An index can be specified to find the 1st, 2nd,
3rd... etc. iteration of they key
----------------------------------------------------------*/
string* Arguments::value(string key, int index){
	int current_index = 0;
	for (int i = 0; i < number_of_arguments; i++){
		if (key.compare(*keys[i]) == 0){
			if (current_index == index){
				return values[i];
			}
			else{
				
				current_index++;
			}
		}
	}
	return NULL;
}

/*----------------------------------------------------------
Returns the number of valid arguments passed into the class
----------------------------------------------------------*/
int Arguments::get_number_of_arguments(){
	return number_of_arguments;
}

/*----------------------------------------------------------
Adds a custom argument pair to the list
----------------------------------------------------------*/
void Arguments::add_args(string* arg){
	string** keyvalue = argument_to_keyvalue(arg);
	keys[get_number_of_arguments()] = keyvalue[0];
	values[get_number_of_arguments()] = keyvalue[1];
	++number_of_arguments;

}
