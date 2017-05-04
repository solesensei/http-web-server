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
		dec ( cur_type );
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
	//add postfix
	prefix();
	simple_expression();
	infix();
	// check_op();
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
        check_id();
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
        st_lex.push(cur_type); // push operation to stack
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
    st_int.reset();
	if(cur_type==LEX_ID){
		st_int.push ( cur_value );
		check_id(); //push var to stack 
		get_lexem();
		if(cur_type==LEX_EQ){
			get_lexem();
			expression();
			eq_type();
		}
		else if(cur_type==LEX_SEMICOLON){
			get_lexem();
			return;
		}
		while(cur_type!=LEX_SEMICOLON){
			if(cur_type==LEX_COMMA){
				get_lexem();
				if(cur_type==LEX_ID){
                    st_int.push ( cur_value );
					check_id();
					get_lexem();
					if(cur_type==LEX_EQ){
						get_lexem();
						simple_expression();
						eq_type();
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
		eq_bool();
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
			eq_bool();
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
				eq_bool();
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
			eq_bool();
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

/* SEMANTIC */

void Parser::dec ( type_of_lexem type ){
    int i;
    while ( !st_int.is_empty()){
        
        i = st_int.pop();
        if ( TID[i].get_declare() )
            throw "twice";
        else
        {
            TID[i].set_declare();
            TID[i].set_type(type);
        }
    }
}

void Parser::check_id(){
    if ( TID[cur_value].get_declare() )
        st_lex.push(TID[cur_value].get_type());
    else
        throw "not declared";
}

/* add boolean lexem ? 
void Parser::check_op (){ //add boolean ?
    type_of_lexem t1, t2, op, t = LEX_VAR, r = LEX_BOOL;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if ( op==LEX_PLUS || op==LEX_MINUS || op==LEX_TIMES || op==LEX_SLASH )
        r = LEX_VAR;
   // if ( op == LEX_OR || op == LEX_AND )
   //     t = LEX_BOOL;
    if ( t1 == t2 && t1 == t )
        st_lex.push(r);
    else
        throw "wrong types are in operation";
}
*/

void Parser::eq_type (){
    if ( st_lex.pop() != st_lex.pop() ) throw "wrong types are in =";
}

void Parser::eq_bool (){
    if ( st_lex.pop() != LEX_BOOL )
        throw "expression is not boolean";
}