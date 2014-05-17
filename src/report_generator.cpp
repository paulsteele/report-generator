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
namespace report_generator {

	string* ask_input(string field){
		cout << "Please enter desired text for " << field << "\n";
		string* text = new string();
		std::getline(cin, *text);
		return text;
	}

	void cleanup_fills(list<string**>* fills){
		while (fills->size() > 0){
			delete fills->front()[0];
			delete fills->front()[1];
			delete[] fills->front();
			fills->pop_front();
		}
	}

	string* find_match(list<string**>* fills, string* match){
		for (list<string**>::iterator i = fills->begin(); i != fills->end(); i++){
			if (match->compare(*((*i)[0])) == 0){
				return (*i)[1];
			}
		}
		return NULL;
	}

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

	void parse_template(string* file, list<string**>* fills){
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
						if (enterinfo.compare("n") == 0){
							breakout = true;
						}
						cin.clear();
						cin.ignore();
					}
					iteration++;
				}

				multifield.clear();
			}
			else if (!inside_multi){
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

	bool ask_execution(string execution){
		cout << "-----\nExecute '" << execution << "'?(y/n)\n";
		string response;
		cin >> response;
		cin.clear();
		cin.ignore();
		if (response.at(0) == 'y')
			return true;
		else
			return false;
	}

	void file_system_calls(string file, bool ask_for_execution, int file_num = -1){
		if (file_num == -1){ //DO ALL THE COMMANDS
			for (int i = 0; i < NUM_FILE_COMMANDS; i++){
				string execution = file_commands[i];
				execution += file;
				if (!ask_for_execution || ask_execution(execution))
					system(execution.c_str());
			}
		}
		else {
			string execution = file_commands[file_num];
			execution += file;
			if (!ask_for_execution || ask_execution(execution))
				system(execution.c_str());
		}
	}

	void norm_system_calls( bool ask_for_execution, int file_num = -1){
		if (file_num == -1){
			for (int i = 0; i < NUM_NORM_COMMANDS; i++){
				string execution = norm_commands[i];
				if (!ask_for_execution || ask_execution(execution))
					system(execution.c_str());
			}
		}
		else {
			string execution = norm_commands[file_num];
			if (!ask_for_execution || ask_execution(execution))
				system(execution.c_str());
		}
	}
}
	
int main(int argc, char** argv) {
	Arguments* args = new Arguments(argc, argv);
	string* file = args->value(string("f"));
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
		cout << "-----\nParsing " << *file << "\n-----\n";
		report_generator::parse_template(file, fills);
		report_generator::file_system_calls(*file, ask_for_execution, i++);
		file = args->value(string("f"), i); //note this is the NEXT i considering i++ above

	}
	report_generator::norm_system_calls(ask_for_execution);
	report_generator::cleanup_fills(fills);
	delete fills;
	delete args;
}
