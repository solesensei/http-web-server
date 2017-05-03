#include <iostream>
#include <fstream>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

int main(){
	Parser p(PROG_PATH);
	try{
		p.get_lexem();
		p.analyze();
		while(p.cur_type!=LEX_FIN){
			p.analyze();
		}
		cout << "Syntax - OK\n";
	}
	catch(error_msg er){
		cout << "Syntax - not OK Line: "<< p.cur_string_number << endl << er.message;
	}
}