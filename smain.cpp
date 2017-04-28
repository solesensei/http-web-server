#include <iostream>
#include <fstream>
#include "interpretlib.h"

int main(){
	try{
		Parser p(PROG_PATH);
		p.analyze();
	}
	catch(Lexem l){
		cout << "Syntax - not OK" << l << endl;
	}
}