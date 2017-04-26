#include <iostream>
#include <fstream>
#include "interpretlib.h"
using namespace std;


void Parser::analyze(){
	get_lexem();
	sentence();
	cout << "Syntax - OK" << endl;
}

void Parser::sentence(){
	operat();
}

void Parser::operat(){
	if(cur_type==LEX_VAR){
		get_lexem();
		var_definition();
	}
	else if(cur_type==LEX_LBRACE){
		get_lexem();
		block();
		if(cur_type==LEX_RBRACE){
			get_lexem();
		}
		else
			throw current_lexem;
	}
	else if(cur_type==LEX_IF){
		get_lexem();
		condition();
	}
	else if(cur_type==LEX_FOR||cur_type==LEX_WHILE||cur_type==LEX_DO){
		cycle();
	}
	else{
		expression();
		get_lexem();
	}
}

void Parser::block(){
	while(cur_type!=LEX_RBRACE){
		operat();
	}
}

void Parser::expression(){
	//add prefix and postfix
	simple_expression();
	infix();
	/*if(cur_type==LEX_SEMICOLON){
		//get_lexem();
	}*/
}

void Parser::simple_expression(){
	if(cur_type==LEX_ID || cur_type == LEX_NUM || cur_type == LEX_STRING){
		get_lexem();
	}
	else if(cur_type==LEX_LPAREN){
		get_lexem();
		expression();
		if(cur_type==LEX_RPAREN){
			cout << "hehe\n";
			get_lexem();
		}
		else{
			throw current_lexem;
		}
	}
	else{
		throw current_lexem;
	}
}

void Parser::infix(){
	//add all infix operations l8r
	if(cur_type==LEX_PLUS || cur_type==LEX_MINUS || cur_type==LEX_TIMES || cur_type == LEX_SLASH || cur_type==LEX_EQ || \
	   cur_type==LEX_DEQ || cur_type==LEX_TEQ || cur_type==LEX_LSS || cur_type==LEX_GTR || cur_type==LEX_LEQ || \
	   cur_type==LEX_NEQ || cur_type==LEX_GEQ){
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_RPAREN || cur_type==LEX_SEMICOLON){

	}
	else{
		throw current_lexem;
	}
}

void Parser::var_definition(){
	if(cur_type==LEX_ID){
		get_lexem();
		if(cur_type==LEX_EQ){
			get_lexem();
			expression();
		}
		else if(cur_type==LEX_SEMICOLON){
			get_lexem();
			return;
		}
		while(cur_type!=LEX_SEMICOLON){
			if(cur_type==LEX_COMMA){
				get_lexem();
				if(cur_type==LEX_ID){
					get_lexem();
					if(cur_type==LEX_EQ){
						get_lexem();
						simple_expression();
					}
				}
				else{
					throw current_lexem;
				}
			}
			else{
				throw current_lexem;
			}
		}
	}
	else{
		throw current_lexem;
	}
	get_lexem();
}

void Parser::condition(){
	if(cur_type==LEX_LPAREN){
		get_lexem();
		simple_expression();
		if(cur_type==LEX_RPAREN){
			get_lexem();
			operat();
			get_lexem();
			if(cur_type==LEX_ELSE){
				operat();
			}
		}
		else{
			throw current_lexem;
		}
	}
}	

void Parser::cycle(){
	/* for cycle */
	if(cur_type == LEX_FOR){
		get_lexem();
		if(cur_type==LEX_LPAREN){
			get_lexem();
			expression();
			if(cur_type!=LEX_SEMICOLON){
				throw current_lexem;
			}
			get_lexem();
			expression();
			if(cur_type!=LEX_SEMICOLON){
				throw current_lexem;
			}
			get_lexem();
			expression();
			if(cur_type==LEX_RPAREN){
				get_lexem();
				operat();
			}
			else{
				throw current_lexem;
			}

		}
		else{
			throw current_lexem;
		}
	}

	/* do cycle */
	if(cur_type == LEX_DO){
		get_lexem();
		operat();
		if(cur_type==LEX_WHILE){
			get_lexem();
			if(cur_type==LEX_LPAREN){
				get_lexem();
				expression();
				if(cur_type==LEX_RPAREN){
					get_lexem();
					if(cur_type==LEX_SEMICOLON){
						get_lexem();
					}
					else{
						throw current_lexem;
					}
				}
				else{
					throw current_lexem;
				}
			}
			else{
				throw current_lexem;
			}
		}
		else{
			throw current_lexem;
		}
	}

	/* while cycle */
	else if(cur_type == LEX_WHILE){
		get_lexem();
		if(cur_type==LEX_LPAREN){
			get_lexem();
			expression();
			if(cur_type==LEX_RPAREN){
				get_lexem();
				operat();
			}
			else{
				throw current_lexem;
			}
		}
		else{
			throw current_lexem;
		}
	}
}