#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "interpretlib.h"
using namespace std;

// this vector stores gaps to fill in Poliz

vector <int> spaces;
vector <int> values;
vector <Lexem> operations;

bool inside_function = false;
bool func_check = false;

void Parser::analyze(){
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
	spaces.push_back(Poliz.size());
	Poliz.push_back(Lexem(POLIZ_LABEL,0));
	Poliz.push_back(Lexem(POLIZ_GO,0));
	int count_id=0;
	get_lexem();
	if(cur_type==LEX_ID){
		st_int.push( cur_value );
		dec( LEX_FUNCTION );
		TID[current_lexem.get_value()].set_type(LEX_FUNCTION);
		Lexem function_lex=current_lexem;
		get_lexem();
		if(cur_type==LEX_LPAREN){
			get_lexem();
            while(!st_int.empty()) // st_int.reset();
				st_int.pop();
			if(cur_type==LEX_ID){
				count_id++;
				st_int.push ( cur_value );
				dec ( LEX_NULL );
				TID[function_lex.get_value()].id_values.push(current_lexem.get_value());
				get_lexem();
				while(cur_type==LEX_COMMA){
					get_lexem();
					if (cur_type==LEX_ID){
						count_id++;
						st_int.push ( cur_value );
						dec ( LEX_NULL );
						TID[function_lex.get_value()].id_values.push(current_lexem.get_value());
						get_lexem();
					}
					else{
						throw error_msg(string("parameter's name expected\n"),current_lexem);
					}
				}
				if(cur_type==LEX_RPAREN){
					TID[function_lex.get_value()].set_value(count_id);
					get_lexem();
					get_lexem();
					if(cur_type==LEX_FIN){
						throw error_msg("'}' expected\n",current_lexem);
					}
					TID[function_lex.get_value()].set_address(Poliz.size()+1);
					inside_function=true;
					block();
					if(func_check){
							TID[function_lex.get_value()].set_is_func();
							Poliz.push_back(Lexem(LEX_RETURN,-1));
							func_check=false;
					}
					if(cur_type==LEX_RBRACE){
						get_lexem();
						inside_function=false;
						Poliz.push_back(Lexem(POLIZ_LABEL,-1));
						Poliz.push_back(Lexem(POLIZ_GO,0));
						Poliz[spaces.back()]=Lexem(POLIZ_LABEL,Poliz.size()+1);
						spaces.pop_back();	
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
					TID[function_lex.get_value()].set_value(count_id);
					get_lexem();
					get_lexem();
					if(cur_type==LEX_FIN){
						throw error_msg("'}' expected\n",current_lexem);
					}
					TID[function_lex.get_value()].set_address(Poliz.size()+1);
					inside_function=true;
					block();
					if(func_check){
							TID[function_lex.get_value()].set_is_func();
							Poliz.push_back(Lexem(LEX_RETURN,-1));
							func_check=false;
					}
					if(cur_type==LEX_RBRACE){
						get_lexem();
						inside_function=false;
						Poliz.push_back(Lexem(POLIZ_LABEL,-1));
						Poliz.push_back(Lexem(POLIZ_GO,0));
						Poliz[spaces.back()]=Lexem(POLIZ_LABEL,Poliz.size()+1);
						spaces.pop_back();
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

void Parser::function_call(){
	int count_id = TID[current_lexem.get_value()].get_value();
	bool is_func = TID[current_lexem.get_value()].get_is_func();
	int call_address = TID[current_lexem.get_value()].get_address();
	int call_num =count_id;
	get_lexem();
	if(cur_type==LEX_LPAREN){
		get_lexem();

		/* here check how many arguments, maybe store number in some vector< int > after function() then check here 2 numbers */
		/*while((cur_type==LEX_ID) && TID[current_lexem.get_value()].get_type()!=LEX_FUNCTION){
			count_id--;
			Poliz.push_back(current_lexem);
			get_lexem();
			if(cur_type==LEX_COMMA){
				get_lexem();
			}
			else if(cur_type==LEX_RPAREN){
				break;
			}
			else if(cur_type==LEX_SEMICOLON){
				break;
			}
			else{
				throw error_msg(string("function call syntax error!\n"),current_lexem);
			}
		}*/
		if(cur_type==LEX_RPAREN){
			get_lexem();
			Poliz.push_back(Lexem(POLIZ_LABEL,call_address));
			Poliz.push_back(Lexem(POLIZ_CALL,call_num+1));
			if(is_func){
				Poliz.push_back(Lexem(LEX_NUM,123));
			}
			return;
		}
		else{
			expression();
			count_id--;
			while(cur_type==LEX_COMMA){
				get_lexem();
				expression();
				count_id--;
			}
			if(cur_type==LEX_RPAREN){
				get_lexem();
				//cout << call_address << endl;
				//cout << call_num << endl;
				Poliz.push_back(Lexem(POLIZ_LABEL,call_address));
				Poliz.push_back(Lexem(POLIZ_CALL,call_num+1));
				if(is_func){
					Poliz.push_back(Lexem(LEX_NUM,123));
				}
			}
			else{
				throw error_msg(string("')' expected!\n"),current_lexem);
			}
		}
		if(count_id!=0){
			throw "function call error: wrong number of parameters\n";
		}
		if(cur_type==LEX_SEMICOLON){
			return;
		}
		else{
			throw error_msg(string("';' expected!\n"),current_lexem);
		}


	}
	else{
		throw error_msg(string("'(' expected!\n"),current_lexem);
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
	else if(cur_type==LEX_ALERT){
		get_lexem();
		alert();
	}
	else if(cur_type==LEX_READ){
		get_lexem();
		read();
	}
	else if(cur_type==LEX_FIN){
		return;
	}
	else if(cur_type==LEX_ID){
		if(!vc_lex.empty()){
			if(vc_lex[current_lexem.get_value()-1]==LEX_FUNCTION){
				function_call();	
			}
			else{
				expression();
				Poliz.push_back(Lexem(LEX_SEMICOLON));
				get_lexem();
			}
		}
		else{
			throw "identificator not defined!\n";
		}
		
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
	while(!operations.empty() && operations.back().get_type()!=LEX_LPAREN){
		Poliz.push_back(operations.back());
		operations.pop_back();
	}
}

void Parser::prefix(){
	if(cur_type==LEX_MINUS || cur_type == LEX_PLUS){
		// Here uno MINUS and PLUS 
        if (cur_type == LEX_MINUS){
			get_lexem();
			if (cur_type == LEX_NUM || cur_type==LEX_BOOL || cur_type==LEX_STRING){
				Poliz.push_back(current_lexem);
				Poliz.push_back(LEX_UNOMINUS);
				get_lexem();
				if(cur_type==LEX_EQ){
					throw error_msg(string("expression before '='!\n"),current_lexem);
				}
			}
			else if(cur_type==LEX_ID){
				if(TID[current_lexem.get_value()].get_type()!=LEX_FUNCTION){
					Poliz.push_back(current_lexem);
					Poliz.push_back(Lexem(LEX_UNOMINUS));
					get_lexem();
					if(cur_type==LEX_EQ){
						throw error_msg(string("expression before '=' !\n"),current_lexem);
					}
				}
				else{
					if(TID[current_lexem.get_value()].get_is_func()){
						function_call();
						Poliz.push_back(LEX_UNOMINUS);
					}
					else{
						throw "void function returns nothing!\n";
					}
				}

			}
			else if(cur_type==LEX_LPAREN){
					expression();
					Poliz.push_back(LEX_UNOMINUS);
			}
		}
		else if(cur_type==LEX_PLUS){
			get_lexem();
			if(cur_type==LEX_LPAREN){
				expression();
				Poliz.push_back(LEX_UNOPLUS);
			}
			else if(cur_type==LEX_NUM || cur_type==LEX_STRING || cur_type==LEX_BOOL){
				Poliz.push_back(current_lexem);
				Poliz.push_back(Lexem(LEX_UNOPLUS));
				get_lexem();
				if(cur_type==LEX_EQ){
					throw error_msg(string("expression before '=' !\n"),current_lexem);
				}
			}
			else if(cur_type==LEX_ID){
				if(TID[current_lexem.get_value()].get_type()!=LEX_FUNCTION){
					Poliz.push_back(current_lexem);
					Poliz.push_back(Lexem(LEX_UNOPLUS));
					get_lexem();
					if(cur_type==LEX_EQ){
						throw error_msg(string("expression before '=' !\n"),current_lexem);
					}
				}
				else{
					if(TID[current_lexem.get_value()].get_is_func()){
						function_call();
						Poliz.push_back(LEX_UNOPLUS);
					}
					else{
						throw "void function returns nothing!\n";
					}
				}
			}
		}
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
	if(cur_type == LEX_NUM || cur_type == LEX_STRING){
		st_lex.push( cur_type );
		dec ( cur_type );
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
	else if(cur_type == LEX_ID && TID[current_lexem.get_value()].get_type()!=LEX_FUNCTION){
		check_id();
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
	else if(cur_type == LEX_ID && TID[current_lexem.get_value()].get_type()==LEX_FUNCTION){
		if(TID[current_lexem.get_value()].get_is_func()){
			function_call();
		}
		else{
			throw "Void function returns nothing!\n";
		}
	}
	else if(cur_type==LEX_LPAREN){
		operations.push_back(current_lexem);
		get_lexem();
		expression();
		if(cur_type==LEX_RPAREN){
			if(!operations.empty()){
				while(operations.back().get_type()!=LEX_LPAREN){
					Poliz.push_back(operations.back());
					operations.pop_back();
				}
				operations.pop_back();
			}
			get_lexem();
		}
		else{
			throw error_msg(string("')' expected\n"),current_lexem);
		}
	}
	else if(cur_type==LEX_SEMICOLON || cur_type==LEX_RPAREN){
		return;
	}
	else if(cur_type==LEX_PLUS || cur_type==LEX_MINUS){
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
				  operations.back().get_type()==LEX_OR || operations.back().get_type()==LEX_UNOPLUS|| \
				  operations.back().get_type()==LEX_UNOMINUS){
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
				  operations.back().get_type()==LEX_NEQ || operations.back().get_type()==LEX_AND || \
				  operations.back().get_type()==LEX_UNOPLUS || operations.back().get_type()==LEX_UNOMINUS){
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
				  operations.back().get_type()==LEX_NEQ || operations.back().get_type()==LEX_UNOPLUS || \
				  operations.back().get_type()==LEX_UNOMINUS){
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
				  operations.back().get_type()==LEX_PLUS || operations.back().get_type()==LEX_MINUS || \
				  operations.back().get_type()==LEX_UNOPLUS || operations.back().get_type()==LEX_UNOMINUS){
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
			while(operations.back().get_type()==LEX_TIMES || operations.back().get_type()==LEX_SLASH ||\
				  operations.back().get_type()==LEX_UNOPLUS || operations.back().get_type()==LEX_UNOMINUS){
				Poliz.push_back(operations.back());
				operations.pop_back();
			}
		}
		operations.push_back(current_lexem);
		get_lexem();
		expression();
	}
	else if(cur_type==LEX_RPAREN || cur_type==LEX_SEMICOLON ||cur_type==LEX_COMMA){
		return;
	}
	else{
		throw error_msg(string("infix expression expected\n"),current_lexem);
	}
}


void Parser::var_definition(){
    while(!st_int.empty()) // st_int.reset();
		   st_int.pop();
	if(cur_type==LEX_ID){
		st_int.push( cur_value );
		Poliz.push_back(Lexem(POLIZ_ADDRESS,cur_value));
		get_lexem();
		if(cur_type==LEX_EQ){
			get_lexem();
			dec( cur_type );
			expression();
			Poliz.push_back(Lexem(LEX_EQ));
			Poliz.push_back(Lexem(LEX_SEMICOLON));
		}
		else if(cur_type==LEX_SEMICOLON){
            dec( LEX_NULL );
            TID[current_lexem.get_value()].set_assign();
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
		Poliz.push_back(Lexem(POLIZ_LABEL,0));
		Poliz.push_back(POLIZ_FGO);
		int p;
		if(cur_type==LEX_RPAREN){
			get_lexem();
			operat();
			p = spaces.back();
			spaces.pop_back();
			Poliz[p]=Lexem(POLIZ_LABEL,Poliz.size()+1);
			if(cur_type==LEX_ELSE){
				Poliz[p]=Lexem(POLIZ_LABEL,Poliz.size()+3);
				spaces.push_back(Poliz.size());
				Poliz.push_back(Lexem(POLIZ_LABEL,0));
				Poliz.push_back(POLIZ_GO);
				get_lexem();
				operat();
				int h = spaces.back();
				spaces.pop_back();
				Poliz[h]=Lexem(POLIZ_LABEL,Poliz.size()+1);
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
			Poliz.push_back(Lexem(LEX_SEMICOLON));
			int v1=Poliz.size()+1;//value 1
			if(cur_type!=LEX_SEMICOLON){
				throw error_msg(string("';' expected\n"),current_lexem);
			}
			get_lexem();
			expression();
			eq_bool();
			int s1 = Poliz.size();
			Poliz.push_back(Lexem(POLIZ_LABEL,0));//space1
			Poliz.push_back(POLIZ_FGO);
			if(cur_type!=LEX_SEMICOLON){
				throw error_msg(string("';' expected\n"),current_lexem);
			}
			get_lexem();
			int s2 = Poliz.size();
			Poliz.push_back(Lexem(POLIZ_LABEL,0));//space2
			Poliz.push_back(POLIZ_GO);
			int v2 =Poliz.size()+1;//value 2
			expression();
			Poliz.push_back(Lexem(LEX_SEMICOLON));
			int s3 = Poliz.size();
			Poliz.push_back(Lexem(POLIZ_LABEL,0));//space3
			Poliz.push_back(POLIZ_GO);
			if(cur_type==LEX_RPAREN){
				int v3 = Poliz.size()+1;//value3
				
				
				get_lexem();
				operat();
				
				Poliz.push_back(Lexem(POLIZ_LABEL,v2));
				Poliz.push_back(POLIZ_GO);
				Poliz[s3]=Lexem(POLIZ_LABEL,v1);
				Poliz[s2]=Lexem(POLIZ_LABEL,v3);
				Poliz[s1]=Lexem(POLIZ_LABEL,Poliz.size()+1);
				

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
		spaces.push_back(Poliz.size()+1);
		operat();
		if(cur_type==LEX_WHILE){
			get_lexem();
			if(cur_type==LEX_LPAREN){
				get_lexem();
				expression();
				eq_bool();
				spaces.push_back(Poliz.size());//empty Num
				Poliz.push_back(Lexem(POLIZ_LABEL,0));
				Poliz.push_back(POLIZ_FGO);
				int p = spaces.back();
				spaces.pop_back();
				Poliz[p]=Lexem(POLIZ_LABEL,Poliz.size()+3);
				Poliz.push_back(Lexem(POLIZ_LABEL,spaces.back()));
				spaces.pop_back();
				Poliz.push_back(POLIZ_GO);
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
			spaces.push_back(Poliz.size()+1);//return number
			get_lexem();
			expression();
			eq_bool();
			spaces.push_back(Poliz.size());//empty Num
			Poliz.push_back(Lexem(POLIZ_LABEL,0));
			Poliz.push_back(POLIZ_FGO);
			if(cur_type==LEX_RPAREN){

				get_lexem();
				operat();
				int p = spaces.back();
				spaces.pop_back();
				Poliz[p]=Lexem(POLIZ_LABEL,Poliz.size()+3);
				Poliz.push_back(Lexem(POLIZ_LABEL,spaces.back()));
				spaces.pop_back();
				Poliz.push_back(POLIZ_GO);
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
		if(inside_function){
			func_check=true;
			get_lexem();
			expression();
			if(cur_type==LEX_SEMICOLON){
				get_lexem();

			}
			else{
				throw error_msg(string("';' expected\n"),current_lexem);
			}
		}
		else{
			throw "return not inside a function!\n";
		}
	}
}

void Parser::alert(){
	if(cur_type==LEX_LPAREN){
		get_lexem();
		expression();
		Poliz.push_back(Lexem(LEX_ALERT));
		if(cur_type==LEX_RPAREN){
			get_lexem();
			if(cur_type==LEX_SEMICOLON){
				return;
			}
			else{
				throw error_msg(string("';' expected!\n"),current_lexem);
			}
		}
		else{
			throw error_msg(string("')' expected!\n"),current_lexem);
		}
	}
	else{
		throw error_msg(string("'(' expected!\n"),current_lexem);
	}
}

void Parser::read(){
	if(cur_type==LEX_LPAREN){
		get_lexem();
		if(cur_type==LEX_ID){
			Poliz.push_back(Lexem(POLIZ_ADDRESS,cur_value));
			Poliz.push_back(Lexem(LEX_READ));
			get_lexem();
			if(cur_type==LEX_RPAREN){
				get_lexem();
				if(cur_type==LEX_SEMICOLON){
					return;
				}
				else{
					throw error_msg(string("';' expected!\n"),current_lexem);
				}	
			}
			else{
				throw error_msg(string("')' expected!\n"),current_lexem);
			}
		}
		else{
			throw error_msg(string("identificator expected!\n"),current_lexem);
		}
	}
	else{
		throw error_msg(string("'(' expected!\n"),current_lexem);
	}
}