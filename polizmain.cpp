#include <iostream>
#include <fstream>
#include "interpretlib.h"

int main(int argc,char *argv[]){
	//vector<string> TS;  // table of strings
	if(argc>2){
		cout << "Too much arguments!\n";
		return -1;
	}
	else if(argc==1){
		cout << "Too few arguments!\n";
		return -1;
	}
	Parser par(argv[1]);
	try{
		par.interpretation();
		cout << endl << endl << endl;
		cout << "Syntax - OK\n";
		cout << "Semantic - OK\n";
	}
	catch (char c){
		cout << "Lex error - unexpected char: " << c << endl;
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