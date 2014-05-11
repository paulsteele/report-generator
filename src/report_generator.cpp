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

	void parse_template(string* file, list<string*>* fills){
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
				string* entered = ask_input(field);
				fills->push_back(entered);
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

	void system_calls(string file){
		for (int i = 0; i < NUM_FILE_COMMANDS; i++){
			system((file_commands[i] += file).c_str());
		}
		for (int i = 0; i < NUM_NORM_COMMANDS; i++){
			system((norm_commands[i]).c_str());
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
	list<string*>* fills = new list<string*>;
	report_generator::parse_template(file, fills);
	report_generator::system_calls(*file);
	delete fills;
	delete args;
}
