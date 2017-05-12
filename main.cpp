#include <iostream>
#include <fstream>
#include <stack>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif


class Interpretator{
	Lexem cur_poliz_lexem;
	Parser parser;
 public:
 	Interpretator(char *program):parser(program){}
 	void execute(vector<Lexem> prog);
 	void interpretation(){
 		parser.analyze();
 		execute(parser.Poliz);
 	}
};



void Interpretator::execute(vector<Lexem> prog){
	stack <Lexem> args;
	int start_point=0; // change start_point to main function l8r
	int end_point=par.Poliz.size()+1;
	int index=0;
	int i,j;
	Lexem res1,res2;
	Lexem not_used;
	while(index<end_point){

		cur_poliz_lexem=Poliz[index];
		switch(cur_poliz_lexem.get_type()){
			case LEX_BOOL:
			case LEX_NUM:
			case LEX_STRING:
			case POLIZ_LABEL:
				args.push(cur_poliz_lexem);
				break;
			case LEX_ID:{
				i=cur_poliz_lexem.get_value();
				if(TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_assign()){
						args.push(Lexem(TID[i].get_type(),TID[i].get_value()));
					}
				}
				break;
			}
			case LEX_OR:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res2=Lexem(LEX_BOOL,1);
						}
						else(TID[i].get_value()==0){
							res2=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res2.get_type()==LEX_NULL || (res2.get_type==LEX_NUM && res2.get_value()==0) ||\
				   (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()==0)){
					res2 = Lexem(LEX_BOOL,0);
				}
				else if((res2.get_type()==LEX_NUM && res2.get_value()!=0) ||\
					    (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()>0)){
					res2 = Lexem(LEX_BOOL,1);
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res1=Lexem(LEX_BOOL,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res1.get_type()==LEX_NULL || (res1.get_type==LEX_NUM && res1.get_value()==0) ||\
				   (res1.get_type()==LEX_STRING && parser.get_scanner().TS[res1.get_value()-1].length()==0)){
					res1 = Lexem(LEX_BOOL,0);
				}
				else if((res1.get_type()==LEX_NUM && res1.get_value()!=0) ||\
					    (res1.get_type()==LEX_STRING && parser.get_scanner().TS[res1.get_value()-1].length()>0)){
					res1 = Lexem(LEX_BOOL,1);
				}
				if(res1.get_type()==LEX_BOOL && res2.get_type()==LEX_BOOL){
					args.push(Lexem(LEX_BOOL,res1.get_value() || res2.get_value()));
				}
				else{
					throw "misstake";
				}
				break;
			}
			case LEX_AND:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res2=Lexem(LEX_BOOL,1);
						}
						else(TID[i].get_value()==0){
							res2=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res2.get_type()==LEX_NULL || (res2.get_type==LEX_NUM && res2.get_value()==0) ||\
				   (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()==0)){
					res2 = Lexem(LEX_BOOL,0);
				}
				else if((res2.get_type()==LEX_NUM && res2.get_value()!=0) ||\
					    (res2.get_type()==LEX_STRING && parser.get_scanner().TS(res2.get_value()).length()>0)){
					res2 = Lexem(LEX_BOOL,1);
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res1=Lexem(LEX_BOOL,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res1.get_type()==LEX_NULL || (res1.get_type==LEX_NUM && res1.get_value()==0) ||\
				   (res1.get_type()==LEX_STRING && parser.get_scanner().TS[res1.get_value()-1].length()==0)){
					res1 = Lexem(LEX_BOOL,0);
				}
				else if((res1.get_type()==LEX_NUM && res1.get_value()!=0) ||\
					    (res1.get_type()==LEX_STRING && parser.get_scanner().TS(res1.get_value()).length()>0)){
					res1 = Lexem(LEX_BOOL,1);
				}


				if(res1.get_type()==LEX_BOOL && res2.get_type()==LEX_BOOL){
					args.push(Lexem(LEX_BOOL,res1.get_value() || res2.get_value()));
				}
				else{
					throw "misstake";
				}
				break;
			}



			case LEX_SEMICOLON:{
				/*????*/
				not_used=args.pop();
				break;
			}


			case POLIZ_GO:{
				index=args.pop().get_value()-1;
				break;
			}
			case POLIZ_FGO:{
				i=args.pop().get_value();
				if(!args.pop().get_value()){
					index=i-1;
				}
			}
			case LEX_ALERT:{
				res1 = args.pop();
				i=args.pop().get_value();
				if(res1.get_type()==LEX_NUM || res1.get_type()==LEX_BOOL){
					cout << res1.get_value();
				}
				else if(res1.get_type()==LEX_STRING){
					cout << parser.get_scanner().TS[(res2.get_value())-1];
				}
				//else misstake
				break;
			}
			case LEX_READ:{
				string s;
				int n;
				i = args.pop().get_value();
				if(TID[i].get_value()==LEX_NULL || TID[i].get_value()==LEX_STRING){
					cin >> s;
					parser.get_scanner().TS.push_back(s);

					TID[i].put_value(parser.get_scanner().TS.size());
					TID[i].put_assign();
					break;
				}
			}
			case LEX_MINUS:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						res2 = Lexem(LEX_NUM,TID[i].get_value());
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				else if(res2.get_type()==LEX_STRING){
					res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res2.get_type()==LEX_BOOL){
					res2=Lexem(LEX_NUM,res2.get_value());
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				else if(res1.get_type()==LEX_STRING){
					res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res1.get_type()==LEX_BOOL){
					res1=Lexem(LEX_NUM,res2.get_value());
				}
				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_NUM,res1.get_value()-res2.get_value()));
				}
				//else error
				break;
			}

			case LEX_PLUS:{
				res1 = args.pop();
				i=res1.get_value();
				res2 = args.pop();
				j=res2.get_value();
				bool flag = false;
				if(res2.get_type()==LEX_ID){
					if(TID[i].get_type()==LEX_STRING){
						flag=true;
						res2 = Lexem(LEX_STRING,TID[i].get_value());
					}
				}
				else if(res2.get_type()==LEX_STRING){
					flag=true;
				}

				if(res1.get_type()==LEX_ID){
					if(TID[j].get_type()==LEX_STRING){
						flag=true;
						res1 = Lexem(LEX_STRING,TID[j].get_value());
					}
				}
				else if(res1.get_type()==LEX_STRING){
					flag=true;
				}

				if(res2.get_type()==LEX_ID){
					if(TID[i].get_type()==LEX_BOOL){
						if(flag){
							//parser.get_scanner().TS.push_back(to_string(TID[i].get_value()));
							ostringstream s;
							int i;
							s << i;
							string converted(s.str());
							res2=Lexem(LEX_STRING,TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,TID[i].get_value());
						}
					}
					else if(TID[i].get_type()==LEX_NUM){
						if(flag){
							parser.get_scanner().TS.push_back(to_string(TID[i].get_value()));
							res2=Lexem(LEX_STRING,TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,TID[i].get_value());
						}
					}
					else if(TID[i].get_type()==LEX_NULL){
						if(flag){
							parser.get_scanner().TS.push_back(string(""));
							res2=Lexem(LEX_STRING,TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,0);
						}
					}
				}
				else if(res2.get_type()==LEX_BOOL){
					if(flag){
						parser.get_scanner().TS.push_back(to_string(i));
						res2=Lexem(LEX_STRING,TS.size());
					}
					else{
						res2=Lexem(LEX_NUM,i);
					}
				}
				else if(res2.get_type()==LEX_NUM){
					if(flag){
						parser.get_scanner().TS.push_back(to_string(i));
						res2=Lexem(LEX_STRING,TS.size());
					}
				}

				if(res1.get_type()==LEX_ID){
					if(TID[j].get_type()==LEX_BOOL){
						if(flag){
							parser.get_scanner().TS.push_back(to_string(TID[j].get_value()));
							res1=Lexem(LEX_STRING,TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,TID[j].get_value());
						}
					}
					else if(TID[j].get_type()==LEX_NUM){
						if(flag){
							parser.get_scanner().TS.push_back(to_string(TID[j].get_value()));
							res1=Lexem(LEX_STRING,TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,TID[j].get_value());
						}
					}
					else if(TID[j].get_type()==LEX_NULL){
						if(flag){
							parser.get_scanner().TS.push_back(string(""));
							res1=Lexem(LEX_STRING,TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,0);
						}
					}
				}
				else if(res1.get_type()==LEX_BOOL){
					if(flag){
						parser.get_scanner().TS.push_back(to_string(j));
						res1=Lexem(LEX_STRING,TS.size());
					}
					else{
						res2=Lexem(LEX_NUM,j);
					}
				}
				else if(res1.get_type()==LEX_NUM){
					if(flag){
						parser.get_scanner().TS.push_back(to_string(j));
						res1=Lexem(LEX_STRING,TS.size());
					}
				}


				if(flag){
					if(res1.get_type()==LEX_STRING && res2.get_type()==LEX_STRING){
						parser.get_scanner().TS.push_back(parser.get_scanner().TS[res1.get_value()-1] + parser.TS[res2.get_value(-1)]);
						args.push(LEX_STRING,TS.size());
					}
					else{
						throw "error";
					}
				}
				else{
					if(res1.get_type()==LEX_NUM && res2.get_type()==LEX_NUM){
						args.push(LEX_NUM,res1.get_value()+res2.get_value());
					}
					else{
						throw "error";
					}
				}
				break;
			}
			case LEX_TIMES:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res2=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_NUM,res1.get_value()*res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_DEQ:{
				/* string equality */
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res2=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value()==res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_TEQ:{
				res2 = args.pop();
				res1 = args.pop();
				args.push(Lexem(LEX_BOOL,res1.get_type() == res2.get_type()));
			}
			case LEX_LSS:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						res2 = Lexem(LEX_NUM,TID[i].get_value());
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				else if(res2.get_type()==LEX_STRING){
					res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res2.get_type()==LEX_BOOL){
					res2=Lexem(LEX_NUM,res2.get_value());
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				else if(res1.get_type()==LEX_STRING){
					res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res1.get_type()==LEX_BOOL){
					res1=Lexem(LEX_NUM,res2.get_value());
				}


				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value() < res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_GTR:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						res2 = Lexem(LEX_NUM,TID[i].get_value());
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				else if(res2.get_type()==LEX_STRING){
					res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res2.get_type()==LEX_BOOL){
					res2=Lexem(LEX_NUM,res2.get_value());
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				else if(res1.get_type()==LEX_STRING){
					res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res1.get_type()==LEX_BOOL){
					res1=Lexem(LEX_NUM,res2.get_value());
				}


				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value() > res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_LEQ:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						res2 = Lexem(LEX_NUM,TID[i].get_value());
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				else if(res2.get_type()==LEX_STRING){
					res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res2.get_type()==LEX_BOOL){
					res2=Lexem(LEX_NUM,res2.get_value());
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				else if(res1.get_type()==LEX_STRING){
					res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res1.get_type()==LEX_BOOL){
					res1=Lexem(LEX_NUM,res2.get_value());
				}


				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value() <= res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_GEQ:{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						res2 = Lexem(LEX_NUM,TID[i].get_value());
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				else if(res2.get_type()==LEX_STRING){
					res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res2.get_type()==LEX_BOOL){
					res2=Lexem(LEX_NUM,res2.get_value());
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[TID[i].get_value()-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				else if(res1.get_type()==LEX_STRING){
					res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
				}
				else if(res1.get_type()==LEX_BOOL){
					res1=Lexem(LEX_NUM,res2.get_value());
				}


				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value() >= res2.get_value()));
				}
				//else error
				break;
			}
			case LEX_NEQ():{
				i=cur_poliz_lexem.get_value();
				res2 = args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res2=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res2=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res2=Lexem(LEX_NUM,0);
					}
				}
				res1 = args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else(TID[i].get_value()==0){
							res1=Lexem(LEX_NUM,0);
						}
					}
					else if(TID[i].get_type()==LEX_STRING){
						res1=Lexem(LEX_NUM,atoi(parser.get_scanner().TS[i-1].c_str()));
					}
					else if(TID[i].get_type()==LEX_NULL){
						res1=Lexem(LEX_NUM,0);
					}
				}
				if(res1.get_type() == LEX_NUM && res2.get_type() == LEX_NUM){
					args.push(Lexem(LEX_BOOL,res1.get_value()!=res2.get_value()));
				}
				//else error
				break;

			}
			case LEX_EQ:{
				res2 = args.pop();
				i=res2.get_value();
				res1 = args.pop();
				j=res1.get_value();
				TID[j].put_type(res2.get_type());
				TID[j].put_value(i);
				TID[j].put_assign();
				break;
			}
			default:{
				throw "POLIZ: unexpected elem";
			}
		}
		index++;
	}
}







int main(){
	try{
		Interpretator I(PROG_PATH);
		I.interpretation();
		return 0;
	}
	catch(invalid_argument& err){
		cerr << "Exception catched : " << err.what() << endl;
	}
	catch(char& c){
		cerr << "Exception catched : lex error : " << c << endl;
	}
	catch(error_msg er){
		cout << "Syntax - not OK Line: "<< par.cur_string_number << " Error: " << er.message;
	}
	catch(const char* er){
		cout << "Semantic - not OK Line: " << par.cur_string_number << " Error: " << er << endl;
	}
}