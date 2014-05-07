/*----------------------------------------------------------
Argument Handler

Parses through arguments and returns an array of valid
"keys" and "values"
----------------------------------------------------------*/
#include "argument_handler"

Arguments::Arguments(int argc, char** argv){
	argv++; //get past the file name argument
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
}

Arguments::~Arguments(){
	for (int i = 0; i < number_of_arguments; i++){
		delete keys[i];
		delete values[i];
	}
	delete[] keys;
	delete[] values;
}

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

string* Arguments::key(int index){
	return keys[index];
}

string* Arguments::value(int index){
	return values[index];
}

int Arguments::get_number_of_arguments(){
	return number_of_arguments;
}
