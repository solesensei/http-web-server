#include <iostream>
#include <fstream>
#include "interpretlib.h"

int main(){
	vector<string> TS;  // table of strings
	ofstream out ("output_file");
 try
 {
	const char* program = PROG_PATH;
	Scanner scanner ( program );
	Lexem lex;
	int k = 0;
	while (lex.get_type() != LEX_FIN)
	{
		if ( ++k % 10 == 0) out << endl;
		lex = scanner.get_lex();
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
	vector <string>::const_iterator p = TS.begin();
 	while(p!=TS.end()){
 		out << i <<" "<< *p << endl;
 		p++;
 		i++;
 	}
	return 0;
}