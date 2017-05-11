#include <iostream>
#include <fstream>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

int main(){
	//vector<string> TS;  // table of strings
	ofstream out ("output_file");
	const char* program = PROG_PATH;
	Scanner scanner ( program );
 try
 {
 	int str_k;
	
	Lexem lex;
	int k = 0;
	while (lex.get_type() != LEX_FIN)
	{
		if ( ++k % 10 == 0) out << endl;
		lex = scanner.get_lex(&str_k);
		out << lex;
	}	
 }

catch(invalid_argument& err)
	{
		cerr << "Exception catched : " << err.what() << endl;
	}
catch(char& c)
	{
		cerr << "Exception catched : lex error : " << c << endl;
	}
	out << endl << endl;
	int i=1;
	vector <string>::const_iterator p = scanner.TS.begin();
 	while(p!=scanner.TS.end()){
 		out << i <<" "<< *p << endl;
 		p++;
 		i++;
 	}
	return 0;
}