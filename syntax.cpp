#include <iostream>
#include <fstream>
#include <string>
#include "interpretlib.h"
using namespace std;



void Parser::analyze(){
	//get_lexem();
	sentence();
}

void Parser::sentence(){
	if(cur_type==LEX_FUNCTION){
		function();
	}
	else{
		operat();
	}
}
void Parser::function(){
	get_lexem();
	if(cur_type==LEX_ID){
		get_lexem();
		if(cur_type==LEX_LPAREN){
			get_lexem();
			if(cur_type==LEX_ID){
				get_lexem();
				while(cur_type==LEX_COMMA){
					get_lexem();
					if (cur_type==LEX_ID){
						get_lexem();
					}
					else{
						throw error_msg(string("parameter's name expected\n"),current_lexem);
					}
				}
				if(cur_type==LEX_RPAREN){
					get_lexem();
					get_lexem();
					if(cur_type==LEX_FIN){
						throw error_msg("'}' expected\n",current_lexem);
					}
					block();
					if(cur_type==LEX_RBRACE){
						get_lexem();
					}
					else{
						throw error_msg(string("'}' expected\n"),current_lexem);
					}
				}
				else{
					throw error_msg(string("')' expected\n"),current_lexem);
				}
			}
			else if(cur_type==LEX_RPAREN){
					get_lexem();
					get_lexem();
					if(cur_type==LEX_FIN){
						throw error_msg("'}' expected\n",current_lexem);
					}
					block();
					if(cur_type==LEX_RBRACE){
						get_lexem();
					}
					else{
						throw error_msg(string("'}' expected\n"),current_lexem);
					}
			}
			else{
				throw error_msg(string("parameter's name expected\n"),current_lexem);
			}
		}
		else{
			throw error_msg(string("'(' expected\n"),current_lexem);
		}
	}
	else{
		throw error_msg(string("function name expected\n"),current_lexem);
	}
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
			throw error_msg(string("'}' expected\n"),current_lexem);
	}
	else if(cur_type==LEX_IF){
		get_lexem();
		condition();
	}
	else if(cur_type==LEX_FOR||cur_type==LEX_WHILE||cur_type==LEX_DO){
		cycle();
	}
	else if(cur_type==LEX_BREAK||cur_type==LEX_CONTINUE||cur_type==LEX_RETURN){
		transition();
	}
	/*empty operator*/
	else if(cur_type==LEX_SEMICOLON){
		get_lexem();
	}
	else if(cur_type==LEX_FIN){
		return;
	}
	else{
		expression();
		get_lexem();
	}
}

void Parser::block(){
	while(cur_type!=LEX_RBRACE){
		operat();
		if(cur_type==LEX_FIN){
			throw error_msg(string("'}' expected\n"),current_lexem);
		}
	}
}

void Parser::expression(){
	//add prefix and postfix
	prefix();
	simple_expression();
	infix();
}
void Parser::prefix(){
	if(cur_type==LEX_MINUS || cur_type == LEX_PLUS){
		get_lexem();
		return;
	}
	else if(cur_type==LEX_ID || cur_type == LEX_NUM || cur_type == LEX_STRING || cur_type==LEX_LPAREN){
		return;
	}
	else{
		throw error_msg(string("prefix expression expected\n"),current_lexem);
	}
}

void Parser::simple_expression(){
	if(cur_type==LEX_ID || cur_type == LEX_NUM || cur_type == LEX_STRING){
		get_lexem();
	}
	else if(cur_type==LEX_LPAREN){
		get_lexem();
		expression();
		if(cur_type==LEX_RPAREN){
			get_lexem();
		}
		else{
			throw error_msg(string("')' expected\n"),current_lexem);
		}
	}
	else if(cur_type==LEX_SEMICOLON){
		return;
	}
	else{
		throw error_msg(string("';' expected\n"),current_lexem);
	}
}

void Parser::infix(){
	if(cur_type==LEX_PLUS || cur_type==LEX_MINUS || cur_type==LEX_TIMES ||\
	   cur_type==LEX_SLASH || cur_type==LEX_EQ || cur_type==LEX_DEQ || \
	   cur_type==LEX_TEQ || cur_type==LEX_LSS || cur_type==LEX_GTR ||\
	   cur_type==LEX_LEQ || cur_type==LEX_NEQ || cur_type==LEX_GEQ){
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_RPAREN || cur_type==LEX_SEMICOLON || cur_type==LEX_FUNCTION || \
			cur_type==LEX_FOR || cur_type==LEX_DO || cur_type==LEX_WHILE || cur_type == LEX_IF){
		return;
	}
	else{
		throw error_msg(string("infix expression expected\n"),current_lexem);
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
					throw error_msg(string("variable's name expected\n"),current_lexem);
				}
			}
			else{
				throw error_msg(string("',' expected\n"),current_lexem);
			}
		}
	}
	else{
		throw error_msg(string("variable's name expected\n"),current_lexem);
	}
	get_lexem();
}

void Parser::condition(){
	if(cur_type==LEX_LPAREN){
		get_lexem();
		expression();
		if(cur_type==LEX_RPAREN){
			get_lexem();
			operat();
			get_lexem();
			if(cur_type==LEX_ELSE){
				operat();
			}
		}
		else{
			error_msg(string("')' expected\n"),current_lexem);
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
				throw error_msg(string("';' expected\n"),current_lexem);
			}
			get_lexem();
			expression();
			if(cur_type!=LEX_SEMICOLON){
				throw error_msg(string("';' expected\n"),current_lexem);
			}
			get_lexem();
			expression();
			if(cur_type==LEX_RPAREN){
				get_lexem();
				operat();
			}
			else{
				throw error_msg(string("')' expected\n"),current_lexem);
			}

		}
		else{
			throw error_msg(string("'(' expected\n"),current_lexem);
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
						throw error_msg(string("';' expected\n"),current_lexem);
					}
				}
				else{
					throw error_msg(string("')' expected\n"),current_lexem);
				}
			}
			else{
				throw error_msg(string("'(' expected\n"),current_lexem);
			}
		}
		else{
			throw error_msg(string(" 'while' expected\n"),current_lexem);
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
				throw error_msg(string("')' expected\n"),current_lexem);
			}
		}
		else{
			throw error_msg(string("'(' expected\n"),current_lexem);
		}
	}
}

void Parser::transition(){
	/*break operator*/
	if(cur_type==LEX_BREAK){
		get_lexem();
		if(cur_type==LEX_SEMICOLON){
			get_lexem();
		}
		else{
			throw error_msg(string("';' expected\n"),current_lexem);
		}
	}
	else if(cur_type==LEX_CONTINUE){
		get_lexem();
		if(cur_type==LEX_CONTINUE){
			get_lexem();
			if(cur_type==LEX_SEMICOLON){
				get_lexem();
			}
			else{
				throw error_msg(string("';' expected\n"),current_lexem);
			}
		}
	}
	else if(cur_type==LEX_RETURN){
		get_lexem();
		expression();
		if(cur_type==LEX_SEMICOLON){
			get_lexem();
		}
		else{
			throw error_msg(string("';' expected\n"),current_lexem);
		}
	}
}