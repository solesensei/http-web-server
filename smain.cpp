#include <iostream>
#include <fstream>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

int main(){
	try{
		Parser p(PROG_PATH);
		p.get_lexem();
		p.analyze();
		while(p.cur_type!=LEX_FIN){
			p.analyze();
		}
		cout << "Syntax - OK\n";
	}
	catch(string str){
		cout << "Syntax - not OK: " << str;
	}
}