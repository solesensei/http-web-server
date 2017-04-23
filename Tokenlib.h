#ifndef TOKENLIB_H
#define TOKENLIB_H

#include <cstring>
#include <vector>
#include <string>
using namespace std;

enum type_of_lexem{
	LEX_NULL, //0
	LEX_AND, //1
	LEX_BREAK, //2
	LEX_CASE, //3
	LEX_CATCH, //4
	LEX_CONST, //5
	LEX_CONTINUE, //6
	LEX_DEBUGGER, //7
	LEX_DEFAULT, //8
	LEX_DELETE, //9
	LEX_DO, //10
	LEX_ELSE, //11
	LEX_FOR, //13
	LEX_FINALLY, //12
	LEX_FUNCTION, //14
	LEX_IF, //15
	LEX_IN, //16
	LEX_INSTANCEOF, //17
	LEX_LET, //18
	LEX_NEW, //19
	LEX_OR, //20
	LEX_RETURN, //21
	LEX_SWITCH, //22
	LEX_THROW, //23
	LEX_TRY, //24
	LEX_TYPEOF, //25
	LEX_VAR, //26
	LEX_VOID, //27
	LEX_WHILE, //28
	LEX_WITH, //29
	LEX_SEMICOLON, //30
	LEX_COMMA, //31
	LEX_COLON, //32
	LEX_LPAREN, //33
	LEX_RPAREN, //34
	LEX_LSS, //35
	LEX_EQ, //36
	LEX_DEQ,//37
	LEX_TEQ, //38
	LEX_GTR, //39
	LEX_PLUS, //40
	LEX_MINUS, //41
	LEX_TIMES, //42
	LEX_SLASH, //43
	LEX_LEQ, //44
	LEX_NEQ, //45
	LEX_GEQ, //46
	LEX_NUM, //47
	LEX_ID, //48
	POLIZ_LABEL, //49
	POLIZ_ADDRESS, //50
	POLIZ_GO, //51
	POLIZ_FGO, //52
	LEX_FIN, //53
	LEX_LBRACE, //54
	LEX_RBRACE, //55
	LEX_STRING, //56
};


/* Lexem is (type of a lexem, value of lexem)*/
class Lexem{
	type_of_lexem type_lex;
	int value_lex;
 public:
 	Lexem(type_of_lexem t = LEX_NULL, int v = 0){
 		type_lex = t;
 		value_lex = v;
 	}
 	type_of_lexem get_type(){ return type_lex; }

 	int get_value(){ return value_lex; }

 	friend ostream& operator<< (ostream &s, Lexem l){
 		s << '(' << l.type_lex << " - " << l.value_lex << "); ";
 		return s;
 	}
};


/* Here identificators described */
class Identificator{
	char *name;
	bool declare;
	type_of_lexem type;
	bool assign;
	int value;
 public:
 	Identificator(){
 		declare = false;
 		assign = false;
 	}
 	char* get_name(){ return name;}
 	void set_name(const char *s){ 
 		name = new char[strlen(s)+1];
 		strcpy(name,s);
	}
	bool get_declare(){ return declare; }
	void set_declare(){ declare = true; }
	type_of_lexem get_type(){ return type; }
	void set_type(type_of_lexem t){ type = t; }
	bool get_assign(){ return assign; }
	void set_assign(){ assign = true; }
	int get_balue(){ return value; }
	void set_value(int k){ value = k; }
};

class Scanner{
	
	enum state{ H , IDENT , NUMB , COM1 , COM2 , ALE , COMP,DELIM , NEQ , STRING,ERROR};
	static const char *TW[];
	static type_of_lexem words[];
	static const char *TD[];
	static type_of_lexem dlms[];
	//static char* TS[];
	state current_state;
	FILE *fp;
	char c;
	char buf[80];
	int buf_top;

	void clear(){
		buf_top = 0;
		for(int j=0; j<80; ++j)
			buf[j]='\0';
	}

	void add(){
		buf[buf_top++]=c;
	}

	int look(const char *buf, const char **list){
		int i = 0;
		while(list[i]){
			if(!strcmp(buf,list[i]))
				return i;
			++i;
		}
		return 0;
	}

	void get_char(){
		c = fgetc(fp);
	}
 public:
	/* set up tables of identificators */
 	Scanner (const char *program){
		if( !(fp = fopen(program,"r")) )
			 throw invalid_argument("no such file");
 		current_state = H;
 		clear();
 		get_char();
 	}
 	Lexem get_lex();
};	

#endif