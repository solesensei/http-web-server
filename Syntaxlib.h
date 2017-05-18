#ifndef SYNTAXLIB_H
#define SYNTAXLIB_H

using namespace std;
#include "interpretlib.h"	
#include <vector>
#include <stack>
#define STL_STACK

#ifndef STL_STACK
template <class T, int max_size > class stack{
	T s[max_size];
	int top;
public:
	stack(){top = 0;}
	void reset() { top = 0; }
	void push(T i);
	T pop();
	bool is_empty(){ return top == 0; }
	bool is_full() { return top == max_size; }
};

template <class T, int max_size >
void stack <T, max_size >::push(T i){
	if ( !is_full() ){
		s[top] = i;
		++top;
	}
	else
		throw "Stack_is_full";
	}
template <class T, int max_size >
T stack <T, max_size >::pop(){
	if ( !is_empty() ){
		--top;
		return s[top];
	}
	else
		throw "Stack_is_empty";
}	
#endif

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
	Lexem cur_poliz_lexem;
	int cur_value;
	Scanner scan;

	//procedures of recursive descent parser//
	void sentence(); //syntax
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
	void alert();
	void read();
	void function_call();
	//semantic
	void dec (type_of_lexem type);
	void udec ();
	void check_id ();
	void check_op ();
	void eq_type ();
	void eq_bool ();
	// void check_id_in_read ();

	//interpretation
	


 public:
 	int cur_string_number;
 	type_of_lexem cur_type;
 	vector<Lexem> Poliz;
	vector<type_of_lexem> vc_lex; 
	stack <int> st_int;
	stack <int> st_loc;
	stack <type_of_lexem> st_lex;


	void get_lexem(){
		current_lexem = scan.get_lex(&cur_string_number);
		cur_type = current_lexem.get_type();
		cur_value = current_lexem.get_value();
	}
	// Poliz prog;
 	Parser(const char* program):scan(program){
 		Poliz.reserve(1000);
 		cur_string_number = 1;
 	}
 	void analyze();
 	void execute();
 	void interpretation();

 	Scanner get_scanner(){
 		return scan;
 	}
 
	void print_vec(vector<type_of_lexem> v) const
	{
		cout << "variable types: " << endl;
		for (vector<type_of_lexem>::const_iterator i = v.begin(); i != v.end(); i++)
		    	cout << *i << ' ';
		cout << endl << endl;
	} 
};
#endif