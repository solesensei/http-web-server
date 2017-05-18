#include <iostream>
#include <fstream>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

int main(){
	//vector<string> TS;  // table of strings
	ofstream out ("output_file");
	Parser par(PROG_PATH);
	try{
		par.interpretation();
		cout << endl << endl << endl;
		cout << "Syntax - OK\n";
		cout << "Semantic - OK\n";
	}
	
	catch(error_msg er){
		cout << "Syntax - not OK Line: "<< par.cur_string_number << " Error: " << er.message << er.error_lex << endl;
	}
	catch(const char* er){
		cout << "Semantic - not OK Line: " << par.cur_string_number << " Error: " << er << endl;
	}
	par.print_vec(par.vc_lex);
	cout << "Poliz:\n";
	vector<Lexem>::const_iterator t = par.Poliz.begin();
	if(t==par.Poliz.end()){
		cout << "Empty Poliz\n";
	}
	while(t!=par.Poliz.end()){
		cout << *t << " ";
		t++;
	}

	cout << endl << endl << "Size: " << par.Poliz.size() << endl;


	return 0;
}