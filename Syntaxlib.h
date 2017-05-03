#ifndef SYNTAXLIB_H
#define SYNTAXLIB_H

#include "interpretlib.h"	
#include <stack>

using namespace std;
struct error_msg{
	string message;
	Lexem error_lex;
	error_msg(string msg,Lexem& er_l){
		message=msg;
		error_lex=er_l;
	}
};

class Parser{
	Lexem current_lexem;
	int cur_value;
	Scanner scan;
	stack<int> st_int;
	stack<type_of_lexem> st_lex;

	//procedures of recursive descent parser//
	void sentence();
	void function();
	void operat();
	void block();
	void var_definition();
	void expression();
	void simple_expression();
	void prefix();
	void infix();
	void condition();
	void cycle();
	void transition();
 public:
 	int cur_string_number;
 	type_of_lexem cur_type;
	void get_lexem(){
		current_lexem = scan.get_lex(&cur_string_number);
		cur_type = current_lexem.get_type();
		cur_value = current_lexem.get_value();
	}

 	Parser(const char* program):scan(program){
 		cur_string_number=0;
 	}
 	void analyze();
};
#endif