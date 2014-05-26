/*----------------------------------------------------------
Report Generator
Project page : https://github.com/paulsteele/report-generator

This file is protected under a Creative Commons 
Attribution-NonCommercial 3.0 Unported License.
For further information see paul-steele.com/Pages/license.php

Reads in a LaTeX source file and will replace ~FIELD~ with
user selected input
----------------------------------------------------------*/
#include "report_generator"

/*----------------------------------------------------------
Main function declaration. 

In this name space the argument 'fills' is a pointer
to a list of string arrays of length two. The first string
is the template field and the second is the replacement
value. The goal of this is to allow multiple templates
with identical fields to be automatically filled.

the fills list is also used to store choices of 
confirmation prompts 
----------------------------------------------------------*/
namespace report_generator {

	/*----------------------------------------------------------
	Takes in a field name, prompts the user for the desired
	replacement text, and returns a pointer to the allocated
	string containing the replacement.
	----------------------------------------------------------*/
	string* ask_input(string field){
		cout << "Please enter desired text for " << field << "\n";
		string* text = new string();
		std::getline(cin, *text);
		return text;
	}

	/*----------------------------------------------------------
	Takes in the fills list and the name of a file and writes
	all of the replacement text values to the file.
	Effectively prints all of the input entered into the program
	----------------------------------------------------------*/
	void write_fills(list<string**>* fills, string file) {
		std::fstream out;
		out.open(file, std::fstream::out);
		for (string** i : *fills){
			out << *(i[1]) << '\n';
		}
		out.close();
	}

	/*----------------------------------------------------------
	frees allocated memory in the fills list
	----------------------------------------------------------*/
	void cleanup_fills(list<string**>* fills){
		while (fills->size() > 0){
			delete fills->front()[0];
			delete fills->front()[1];
			delete[] fills->front();
			fills->pop_front();
		}
	}

	/*----------------------------------------------------------
	Looks for the match argument in the first string of the fills
	list. When it finds a match it returns the second string of
	the fills list. Returns NULL if no match can be found
	----------------------------------------------------------*/
	string* find_match(list<string**>* fills, string* match){
		for (list<string**>::iterator i = fills->begin(); i != fills->end(); i++){
			if (match->compare(*((*i)[0])) == 0){
				return (*i)[1];
			}
		}
		return NULL;
	}

	/*----------------------------------------------------------
	Helper function to deal with parsing
	field = name of the field being dealt with
	inside = whether or not inside a multi line (will prevent immediate output to file)
	autofilled = whether or not this is the second time the field is filled. Allows to bypass confirmation messages
	list = (see namespace comments)
	out = output stream
	field append = the iteration the multiline is on. -1 means it is not a multiline
	----------------------------------------------------------*/
	void parse_helper(string* field, bool* inside, bool* autofilled, char c, list<string**>* fills, std::fstream* out, int field_append = -1){
		
		if (field_append != -1){
			string insert = string("");
			insert+= (field_append + 48);
			insert+= (": ");
			//cout << "inserting " << (char) (field_append + 48) << "to " << *field << '\n';
			field->insert(0, insert);
		}

		if (c == SEPARATOR && !*inside){
			//separator found
			*inside = true;
		}
		else if (c == SEPARATOR && *inside){
			string* entered = find_match(fills, field);
			if (entered == NULL){
				entered = ask_input(*field);
				string** list_container = new string*[2];
				list_container[0] = new string(*field);
				list_container[1] = entered;
				fills->push_back(list_container);
			}
			else{
				*autofilled = true;
			}
			for (int i = 0; i < entered->size(); i++){
				out->put(entered->at(i));
			}
			*inside = false;
			field->clear();
		}
		else if (!*inside){
			out->put(c);
		}
		else if(*inside){
			*field += c;
		}

		if (field_append != -1 && field->compare("") != 0){
			field->erase(0, 3);
		}

	}

	/*----------------------------------------------------------
	Parses a document with help of parse_helper surprisingly enough
	file = name of the file in templates folder
	fills = (see namespace comments)
	it = the number of files already processed (0 means it is first run,
		1 means 2nd run etc)
	multisize = a list of sizes for the multifields. If it = 0
		then multisize is set, otherwise it is used to bypass
		ending checks for multiline confirmations
	----------------------------------------------------------*/
	void parse_template(string* file, list<string**>* fills, int it, list<int>* multisize){
		//Setup Input of file
		std::fstream in;
		string oldfile = string("templates/");
		oldfile += *file;
		in.open(oldfile, std::fstream::in);
		if (!in.good()) {
			//Fail condition
			cout << oldfile << " does not exist. Exiting\n";
			in.close();
			return;
		}
		//Setup Output of file
		std::fstream out;

		string newfile = string("build/");
		newfile += *file;
		out.open(newfile, std::fstream::out);
		int c;
		bool inside_multi = false;
		bool inside = false;
		bool breakout = false;
		bool autofilled = false;
		string field = string("");
		cout << field << "\n";
		string multifield = string("");
		//push through the file
		while ((c = in.get()) != EOF){
			if (c == MULTISEPARATOR && !inside_multi){
				inside_multi = true;
			}
			else if (c == MULTISEPARATOR && inside_multi){
				//the looping here
				breakout = false;
				inside_multi = false;
				autofilled = false;
				int iteration = 1;
				while (!breakout){
					autofilled = false;
					for (int j = 0; j < multifield.size(); j++){
						parse_helper(&field, &inside, &autofilled, multifield.at(j), fills, &out, iteration);
					}
					if (!autofilled) {
						cout << "Enter 'n' to finish this multi line, or any key to continue\n";
						string enterinfo = string("");
						cin >> enterinfo;

						string** list_container = new string*[2];
						list_container[0] = new string("~");
						list_container[1] = new string("y");
						if (enterinfo.compare("n") == 0){
							delete list_container[1];
							list_container[1] = new string("n");
							breakout = true;
						}
						fills->push_back(list_container);
						cin.clear();
						cin.ignore();
					}
					iteration++;

					if (it != 0 && iteration == multisize->front()){
						multisize->pop_front();
						breakout = true;
					}
				}
				multisize->push_back(iteration);

				multifield.clear();
			}
			else if (!inside_multi) {
				//not in multi
				parse_helper(&field, &inside, &autofilled, c, fills, &out);
			}
			else if (inside_multi){
				multifield += c;
			}
		}

		//cleanup
		in.close();
		out.close();
	}

	/*----------------------------------------------------------
	Prompts the user whether they would like to execute a system
	call specified in config. Returns true if the the command is
	to be run, false otherwise
	execution = name of command to be run
	fills = (see namespace header)
	----------------------------------------------------------*/
	bool ask_execution(string execution, list<string**>* fills){
		cout << "-----\nExecute '" << execution << "'?(y/n)\n";
		string response;
		cin >> response;
		cin.clear();
		cin.ignore();

		string** list_container = new string*[2];
		list_container[0] = new string("~");
		list_container[1] = new string("y");

		if (response.at(0) == 'y'){
			fills->push_back(list_container);
			return true;
		}
		else{
			delete list_container[1];
			list_container[1] = new string("n");
			fills->push_back(list_container);
			return false;
		}
	}

	/*----------------------------------------------------------
	Runs the system calls specified in config. These will have
	the current filename appended to the end of the command
	file = name of the file (the one to be appended)
	ask_for_execution = if false, user will not be prompt
	fills = (see namespace header)
	file_num = which file is being run, if -1 then loops through
		all file system calls
	----------------------------------------------------------*/
	void file_system_calls(string file, bool ask_for_execution, list<string**>* fills, int file_num = -1){
		if (file_num == -1){ //DO ALL THE COMMANDS
			for (int i = 0; i < NUM_FILE_COMMANDS; i++){
				string execution = file_commands[i];
				execution += file;
				if (!ask_for_execution || ask_execution(execution, fills))
					system(execution.c_str());
			}
		}
		else {
			string execution = file_commands[file_num];
			execution += file;
			if (!ask_for_execution || ask_execution(execution, fills))
				system(execution.c_str());
		}
	}

	/*----------------------------------------------------------
	Runs the system calls specified in config.
	ask_for_execution = if false, user will not be prompt
	fills = (see namespace header)
	file_num = which file is being run, if -1 then loops through
		all file system calls
	----------------------------------------------------------*/
	void norm_system_calls(bool ask_for_execution, list<string**>* fills, int file_num = -1){
		if (file_num == -1){
			for (int i = 0; i < NUM_NORM_COMMANDS; i++){
				string execution = norm_commands[i];
				if (!ask_for_execution || ask_execution(execution, fills))
					system(execution.c_str());
			}
		}
		else {
			string execution = norm_commands[file_num];
			if (!ask_for_execution || ask_execution(execution, fills))
				system(execution.c_str());
		}
	}
}
	
int main(int argc, char** argv) {
	Arguments* args = new Arguments(argc, argv);
	string* file = args->value(string("f"));
	list<int> multisize = list<int>();
	if (file == NULL){
		//exit conditions
		cout << "No file specified. Exiting\n";
		delete args;
		return 0;
	}

	bool ask_for_execution = true;
	for (int i = 0; i < args->get_number_of_arguments(); i++){
		if (args->key(i)->compare(string("e")) == 0){
			ask_for_execution = false;
		}
	}

	list<string**>* fills = new list<string**>;
	int i = 0;
	while (file != NULL){

		list<int> unit;
		if (i!=0){
			unit = list<int>(multisize);
		}
		else{
			unit = multisize;
		}

		cout << "-----\nParsing " << *file << "\n-----\n";
		report_generator::parse_template(file, fills, i, &unit);
		if (i==0){
			multisize = unit;
		}
		report_generator::file_system_calls(*file, ask_for_execution, fills, i++);
		file = args->value(string("f"), i); //note this is the NEXT i considering i++ above

	}
	report_generator::norm_system_calls(ask_for_execution, fills);
	report_generator::write_fills(fills, string("input.txt"));
	report_generator::cleanup_fills(fills);
	delete fills;
	delete args;
}
