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
	void parse_template(string* file, list<string*>* fills){
		std::fstream in;
		in.open(*file, std::fstream::in);
		if (!in.good()) {
			cout << *file << " does not exist. Exiting\n";
		}
		in.close();
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
	file->insert(0,"templates/");
	list<string*>* fills = new list<string*>;
	report_generator::parse_template(file, fills);
	delete args;
}
