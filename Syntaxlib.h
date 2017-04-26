#include "Tokenlib.h"
#include <stack>
using namespace std;

class Parser{
	Lexem current_lexem;
	type_of_lexem cur_type;
	int cur_value;
	Scanner scan;
	stack<int> st_int;
	stack<type_of_lexem> st_lex;

	//procedures of recursive descent parser//
	void sentence();
	void operat();
	void block();
	void var_definition();
	void expression();
	void simple_expression();
	void infix();
	void condition();
	///////////
	///////////
	///////////
	///////////
	///////////
	///////////
	///////////
	///////////



 public:
	void get_lexem(){
		current_lexem = scan.get_lex();
		cur_type = current_lexem.get_type();
		cur_value = current_lexem.get_value();
	}

 	Parser(const char* program): scan(program) {}
 	void analyze();
};