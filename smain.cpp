#include <iostream>
#include <fstream>
#include "interpretlib.h"

int main(){
	try{
		Parser p(PROG_PATH);
		p.analyze();
	}
	catch(string str){
		cout << "Syntax - not OK: " << str;
	}
}