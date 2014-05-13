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
		bool inside = false;
		string field = string("");
		//push through the file
		while ((c = in.get()) != EOF){
			if (c == SEPARATOR && !inside){
				//separator found
				inside = true;
			}
			else if (c == SEPARATOR && inside){

				string* entered = find_match(fills, &field);
				if (entered == NULL){
					entered = ask_input(field);
					string** list_container = new string*[2];
					list_container[0] = new string(field);
					list_container[1] = entered;
					fills->push_back(list_container);
				}
				for (int i = 0; i < entered->size(); i++){
					out.put(entered->at(i));
				}
				inside = false;
				field.clear();
			}
			else if (!inside){
				out.put(c);
			}
			else if(inside){
				field += c;
			}
		}

		//cleanup
		in.close();
		out.close();
	}

	void file_system_calls(string file, int file_num = -1){
		if (file_num == -1){ //DO ALL THE COMMANDS
			for (int i = 0; i < NUM_FILE_COMMANDS; i++){
				string execution = file_commands[i];
				execution += file;
				cout << "Execute '" << execution << "'?(y/n)\n";
				string response;
				cin >> response;
				if (response.at(0) == 'y')
					system(execution.c_str());
			}
		}
		else {
			string execution = file_commands[file_num];
			execution += file;
			cout << "Execute '" << execution << "'?(y/n)\n";
			string response;
			cin >> response;
			if (response.at(0) == 'y')
				system(execution.c_str());
		}
	}

	void norm_system_calls(int file_num = -1){
		if (file_num == -1){
			for (int i = 0; i < NUM_NORM_COMMANDS; i++){
				string execution = norm_commands[i];
				cout << "Execute '" << execution << "'?(y/n)\n";
				string response;
				cin >> response;
				if (response.at(0) == 'y')
					system(execution.c_str());
			}
		}
		else {
			string execution = norm_commands[file_num];
			cout << "Execute '" << execution << "'?(y/n)\n";
			string response;
			cin >> response;
			if (response.at(0) == 'y')
				system(execution.c_str());
		}
	}
}
	
int main(int argc, char** argv) {
	Arguments* args = new Arguments(argc, argv);
	string* file = args->value(string("file"));
	if (file == NULL){
		//exit conditions
		cout << "No file specified. Exiting\n";
		delete args;
		return 0;
	}
	list<string**>* fills = new list<string**>;
	report_generator::parse_template(file, fills);
	report_generator::file_system_calls(*file);
	report_generator::norm_system_calls();
	report_generator::cleanup_fills(fills);
	delete fills;
	delete args;
}
