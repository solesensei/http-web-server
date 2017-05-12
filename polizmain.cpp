#include <iostream>
#include <fstream>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

int main(){
	//vector<string> TS;  // table of strings
	ofstream out ("output_file");
 /*try{
 	int str_k;
	const char* program = PROG_PATH;
	Scanner scanner ( program );
	Lexem lex;
	int k = 0;
	while (lex.get_type() != LEX_FIN)
	{
		if ( ++k % 10 == 0) out << endl;
		lex = scanner.get_lex(&str_k);
		out << lex;
	}	
 }

catch(invalid_argument& err){
		cerr << "Exception catched : " << err.what() << endl;
	}
catch(char& c){
		cerr << "Exception catched : lex error : " << c << endl;
	}

	out << endl << endl;
	int i=1;
	vector <string>::const_iterator p = TS.begin();
 	while(p!=TS.end()){
 		out << i <<" "<< *p << endl;
 		p++;
 		i++;
 	}*/
	 /* syntax + semantic  */
	Parser par(PROG_PATH);
	try{
		par.get_lexem();
		par.analyze();
		while(par.cur_type!=LEX_FIN){
			par.analyze();
	}
		cout << "Syntax - OK\n";
		cout << "Semantic - OK\n";
	}
	catch(error_msg er){
		cout << "Syntax - not OK Line: "<< par.cur_string_number << " Error: " << er.message;
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