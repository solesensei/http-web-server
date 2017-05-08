#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "interpretlib.h"
using namespace std;


// this vector stores gaps to fill in Poliz

vector <int> spaces;
vector <Lexem> operations;


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
		st_int.push( cur_value );
		dec( LEX_FUNCTION );
		get_lexem();
		if(cur_type==LEX_LPAREN){
			get_lexem();
			st_int.reset();
			if(cur_type==LEX_ID){
				get_lexem();
				while(cur_type==LEX_COMMA){
					get_lexem();
					if (cur_type==LEX_ID){
						st_int.push ( cur_value );
						dec ( LEX_VAR );
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
		Poliz.push_back(Lexem(LEX_SEMICOLON));
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
	prefix();
	simple_expression();
	infix();
	while(!operations.empty()){
		Poliz.push_back(operations.back());
		operations.pop_back();
	}
}

void Parser::prefix(){
	if(cur_type==LEX_MINUS || cur_type == LEX_PLUS){
		// Here uno MINUS and PLUS 
        if (cur_type == LEX_MINUS){
			get_lexem();
			if (cur_type != LEX_NUM)
				throw "LEX_NUM expected after uno minus"; 
		}
		else get_lexem();
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
        if (cur_type == LEX_ID){
        	check_id();
        }
		else{ 
			st_lex.push( cur_type );
			dec ( cur_type );
		} 
		Lexem temp = current_lexem;
		get_lexem();
		if(cur_type==LEX_EQ){
			Poliz.push_back(Lexem(POLIZ_ADDRESS,temp.get_value()));
			get_lexem();
			expression();
			Poliz.push_back(Lexem(LEX_EQ));
		}
		else{
			Poliz.push_back(temp);
		}
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
	if(cur_type==LEX_OR){
		if(!operations.empty()){
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH ||\
				  operations.back().get_type()==LEX_PLUS || operations.back().get_type()==LEX_MINUS ||\
				  operations.back().get_type()==LEX_DEQ || operations.back().get_type()==LEX_TEQ || \
				  operations.back().get_type()==LEX_LSS || operations.back().get_type()==LEX_LEQ || \
				  operations.back().get_type()==LEX_GTR || operations.back().get_type()==LEX_GEQ || \
				  operations.back().get_type()==LEX_NEQ || operations.back().get_type()==LEX_AND || \
				  operations.back().get_type()==LEX_OR){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_AND){

	   	if(!operations.empty()){
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH ||\
				  operations.back().get_type()==LEX_PLUS || operations.back().get_type()==LEX_MINUS ||\
				  operations.back().get_type()==LEX_DEQ || operations.back().get_type()==LEX_TEQ || \
				  operations.back().get_type()==LEX_LSS || operations.back().get_type()==LEX_LEQ || \
				  operations.back().get_type()==LEX_GTR || operations.back().get_type()==LEX_GEQ || \
				  operations.back().get_type()==LEX_NEQ || operations.back().get_type()==LEX_AND){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_DEQ ||cur_type==LEX_TEQ || cur_type==LEX_LSS ||\
		cur_type==LEX_GTR ||cur_type==LEX_LEQ || cur_type==LEX_NEQ ||\
		cur_type==LEX_GEQ){

	  	if(!operations.empty()){
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH ||\
				  operations.back().get_type()==LEX_PLUS || operations.back().get_type()==LEX_MINUS ||\
				  operations.back().get_type()==LEX_DEQ || operations.back().get_type()==LEX_TEQ || \
				  operations.back().get_type()==LEX_LSS || operations.back().get_type()==LEX_LEQ || \
				  operations.back().get_type()==LEX_GTR || operations.back().get_type()==LEX_GEQ || \
				  operations.back().get_type()==LEX_NEQ){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_PLUS ||cur_type==LEX_MINUS){
	   	if(!operations.empty()){
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH ||\
				  operations.back().get_type()==LEX_PLUS || operations.back().get_type()==LEX_MINUS){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_TIMES ||cur_type==LEX_SLASH){
	   	if(!operations.empty()){
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_RPAREN || cur_type==LEX_SEMICOLON){
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
		Poliz.push_back(Lexem(POLIZ_ADDRESS,cur_value));
		get_lexem();
		if(cur_type==LEX_EQ){
			get_lexem();
			dec( cur_type );
			expression();
			Poliz.push_back(Lexem(LEX_EQ));
		}
		else if(cur_type==LEX_SEMICOLON){
            dec( LEX_NULL );
            Poliz.pop_back();
			get_lexem();
			return;
		}
		while(cur_type!=LEX_SEMICOLON){
			if(cur_type==LEX_COMMA){
				get_lexem();
				if(cur_type==LEX_ID){
                    st_int.push ( cur_value );
					Poliz.push_back(Lexem(POLIZ_ADDRESS,cur_value));
					get_lexem();
					if(cur_type==LEX_EQ){
						get_lexem();
						dec( cur_type );
						simple_expression();
						Poliz.push_back(Lexem(LEX_EQ));
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
		spaces.push_back(Poliz.size());
		Poliz.push_back(Lexem(LEX_NUM,0));
		Poliz.push_back(POLIZ_FGO);
		if(cur_type==LEX_RPAREN){
			get_lexem();
			operat();
			int p = spaces.back();
			spaces.pop_back();
			Poliz[p]=Lexem(LEX_NUM,Poliz.size()+1);

			if(cur_type==LEX_ELSE){
				get_lexem();
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