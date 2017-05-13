#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <cstdlib>
#include "interpretlib.h"
#ifndef PROG_PATH
#define PROG_PATH "./input_file.js" 
#endif

using namespace std;

class Interpretator{
	Lexem cur_poliz_lexem;
	Parser parser;
 public:
 	Parser get_parser(){
 		return parser;
 	}
 	Interpretator(const char *program):parser(program){}
 	void execute(vector<Lexem> prog);
 	void interpretation();
};



void Interpretator::execute(vector<Lexem> prog){
	stack <Lexem> args;
	//int start_point=0; // change start_point to main function l8r
	int end_point=parser.Poliz.size()-1;
	int index=0;
	int i,j;
	Lexem res1,res2;
	Lexem not_used;
	while(index<end_point){

		cur_poliz_lexem=parser.Poliz[index];
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
				res2 = args.top();
				args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res2=Lexem(LEX_BOOL,1);
						}
						else if(TID[i].get_value()==0){
							res2=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res2.get_type()==LEX_NULL || (res2.get_type()==LEX_NUM && res2.get_value()==0) ||\
				   (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()==0)){
					res2 = Lexem(LEX_BOOL,0);
				}
				else if((res2.get_type()==LEX_NUM && res2.get_value()!=0) ||\
					    (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()>0)){
					res2 = Lexem(LEX_BOOL,1);
				}
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res1=Lexem(LEX_BOOL,1);
						}
						else if(TID[i].get_value()==0){
							res1=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res1.get_type()==LEX_NULL || (res1.get_type()==LEX_NUM && res1.get_value()==0) ||\
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
				res2 = args.top();
				args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res2=Lexem(LEX_BOOL,1);
						}
						else if(TID[i].get_value()==0){
							res2=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res2.get_type()==LEX_NULL || (res2.get_type()==LEX_NUM && res2.get_value()==0) ||\
				   (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()==0)){
					res2 = Lexem(LEX_BOOL,0);
				}
				else if((res2.get_type()==LEX_NUM && res2.get_value()!=0) ||\
					    (res2.get_type()==LEX_STRING && parser.get_scanner().TS[res2.get_value()-1].length()>0)){
					res2 = Lexem(LEX_BOOL,1);
				}
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						if(TID[i].get_value()!=0){
							res1=Lexem(LEX_BOOL,1);
						}
						else if(TID[i].get_value()==0){
							res1=Lexem(LEX_BOOL,0);
						}
					}
				}
				else if(res1.get_type()==LEX_NULL || (res1.get_type()==LEX_NUM && res1.get_value()==0) ||\
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



			case LEX_SEMICOLON:{
				args.pop();
				break;
			}


			case POLIZ_GO:{
				index=args.top().get_value()-1;
				args.pop();
				break;
			}
			case POLIZ_FGO:{
				i=args.top().get_value();
				args.pop();
				Lexem temp = args.top();
				args.pop();
				if(temp.get_value()){
					index=i-1;
				}
			}
			case LEX_ALERT:{
				res1 = args.top();
				i = args.top().get_value();
				args.pop();
				if(res1.get_type()==LEX_NUM || res1.get_type()==LEX_BOOL){
					cout << res1.get_value();
				}
				else if(res1.get_type()==LEX_STRING){
					cout << parser.get_scanner().TS[(res1.get_value())-1];
				}
				else if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_NUM){
						cout << TID[i].get_value();
					}
					else if(TID[i].get_type()==LEX_STRING){
						cout << parser.get_scanner().TS[TID[i].get_value()-1];
					}
					else if(TID[i].get_type()==LEX_NUM){
						cout << "Undefined\n";
					}

				}
				//else misstake
				break;
			}
			case LEX_READ:{
				string s;
				i = args.top().get_value();
				args.pop();
				if(TID[i].get_value()==LEX_NULL || TID[i].get_value()==LEX_STRING){
					cin >> s;
					parser.get_scanner().TS.push_back(s);

					TID[i].set_value(parser.get_scanner().TS.size());
					TID[i].set_assign();
					break;
				}
			}
			case LEX_MINUS:{
				i=cur_poliz_lexem.get_value();
				res2 = args.top();
				args.pop();
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
				res1 = args.top();
				args.top();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res1 = args.top();
				args.pop();
				i=res1.get_value();
				res2 = args.top();
				args.pop();
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
							int k = TID[i].get_value();
							s << k;
							string converted(s.str());
							res2=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,TID[i].get_value());
						}
					}
					else if(TID[i].get_type()==LEX_NUM){
						if(flag){
							ostringstream s;
							int k = TID[i].get_value();
							s << k;
							string converted(s.str());

							parser.get_scanner().TS.push_back(converted);
							res2=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,TID[i].get_value());
						}
					}
					else if(TID[i].get_type()==LEX_NULL){
						if(flag){
							parser.get_scanner().TS.push_back(string(""));
							res2=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res2=Lexem(LEX_NUM,0);
						}
					}
				}
				else if(res2.get_type()==LEX_BOOL){
					if(flag){
						ostringstream s;
						int k = i;
						s << k;
						string converted(s.str());
						parser.get_scanner().TS.push_back(converted);
						res2=Lexem(LEX_STRING,parser.get_scanner().TS.size());
					}
					else{
						res2=Lexem(LEX_NUM,i);
					}
				}
				else if(res2.get_type()==LEX_NUM){
					if(flag){
						ostringstream s;
						int k = i;
						s << k;
						string converted(s.str());
						parser.get_scanner().TS.push_back(converted);
						res2=Lexem(LEX_STRING,parser.get_scanner().TS.size());
					}
				}

				if(res1.get_type()==LEX_ID){
					if(TID[j].get_type()==LEX_BOOL){
						if(flag){
							ostringstream s;
							int k = TID[j].get_value();
							s << k;
							string converted(s.str());
							parser.get_scanner().TS.push_back(converted);
							res1=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,TID[j].get_value());
						}
					}
					else if(TID[j].get_type()==LEX_NUM){
						if(flag){
							ostringstream s;
							int k = TID[j].get_value();
							s << k;
							string converted(s.str());
							parser.get_scanner().TS.push_back(converted);
							res1=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,TID[j].get_value());
						}
					}
					else if(TID[j].get_type()==LEX_NULL){
						if(flag){
							parser.get_scanner().TS.push_back(string(""));
							res1=Lexem(LEX_STRING,parser.get_scanner().TS.size());
						}
						else{
							res1=Lexem(LEX_NUM,0);
						}
					}
				}
				else if(res1.get_type()==LEX_BOOL){
					if(flag){
						ostringstream s;
						int k = j;
						s << k;
						string converted(s.str());
						parser.get_scanner().TS.push_back(converted);
						res1=Lexem(LEX_STRING,parser.get_scanner().TS.size());
					}
					else{
						res2=Lexem(LEX_NUM,j);
					}
				}
				else if(res1.get_type()==LEX_NUM){
					if(flag){
						ostringstream s;
						int k = TID[i].get_value();
						s << k;
						string converted(s.str());
						parser.get_scanner().TS.push_back(converted);
						res1=Lexem(LEX_STRING,parser.get_scanner().TS.size());
					}
				}


				if(flag){
					if(res1.get_type()==LEX_STRING && res2.get_type()==LEX_STRING){
						parser.get_scanner().TS.push_back(parser.get_scanner().TS[res1.get_value()-1] + parser.get_scanner().TS[res2.get_value()-1]);
						args.push(Lexem(LEX_STRING,parser.get_scanner().TS.size()));
					}
					else{
						throw "error";
					}
				}
				else{
					if(res1.get_type()==LEX_NUM && res2.get_type()==LEX_NUM){
						args.push(Lexem(LEX_NUM,res1.get_value()+res2.get_value()));
					}
					else{
						throw "error";
					}
				}
				break;
			}
			case LEX_TIMES:{
				i=cur_poliz_lexem.get_value();
				res2 = args.top();
				args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
				res1 = args.top();
				args.pop();
				args.push(Lexem(LEX_BOOL,res1.get_type() == res2.get_type()));
			}
			case LEX_LSS:{
				i=cur_poliz_lexem.get_value();
				res2 = args.top();
				args.pop();
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
			case LEX_NEQ:{
				i=cur_poliz_lexem.get_value();
				res2 = args.top();
				args.pop();
				if(res2.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if(TID[i].get_value()==1){
							res2=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res1 = args.top();
				args.pop();
				if(res1.get_type()==LEX_ID && TID[i].get_type()!=LEX_FUNCTION){
					if(TID[i].get_type()==LEX_BOOL){
						if((TID[i].get_value())==1){
							res1=Lexem(LEX_NUM,1);
						}
						else if(TID[i].get_value()==0){
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
				res2 = args.top();
				args.pop();
				i=res2.get_value();
				res1 = args.top();
				args.pop();
				j=res1.get_value();
				TID[j].set_type(res2.get_type());
				TID[j].set_value(i);
				TID[j].set_assign();
				args.push(res2);
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
	Interpretator I(PROG_PATH);
	try{
		I.get_parser().analyze();
 		while(I.get_parser().cur_type!=LEX_FIN)
			I.get_parser().analyze();
 		vector<Lexem>::const_iterator t = I.get_parser().Poliz.begin();

		if(t==I.get_parser().Poliz.end()){
			cout << "Empty Poliz\n";
		}
		while(t!=I.get_parser().Poliz.end()){
			cout << *t << " ";
			t++;
		}
	}
	catch(invalid_argument& err){
		cerr << "Exception catched : " << err.what() << endl;
	}
	catch(char& c){
		cerr << "Exception catched : lex error : " << c << endl;
	}
	catch(error_msg er){
		cout << "Syntax - not OK Line: "<< I.get_parser().cur_string_number << " Error: " << er.message << er.error_lex << endl;
	}
	catch(const char* er){
		cout << "Semantic - not OK Line: " << I.get_parser().cur_string_number << " Error: " << er << endl;
	}
	return 0;
}