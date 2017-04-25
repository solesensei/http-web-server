#include <iostream>
#include <fstream>
#include "interpretlib.h"

#define PROG_PATH "./input_file" 

using namespace std;

int main(){
	try{
		Parser p(PROG_PATH);
		p.analyze();
	}
	catch(Lexem){
		cout << "Syntax - not OK" << endl;
	}
}