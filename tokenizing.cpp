#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;


/* ALL! javascript types of lexem (some may be useless) */
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
	LEX_FINALLY, //12
	LEX_FOR, //13
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
	LEX_ASSIGN, //33
	LEX_LPAREN, //34
	LEX_RPAREN, //35
	LEX_EQ, //36
	LEX_LSS, //37
	LEX_GTR, //38
	LEX_PLUS, //39
	LEX_MINUS, //40
	LEX_TIMES, //41
	LEX_SLASH, //42
	LEX_LEQ, //43
	LEX_NEQ, //44
	LEX_GEQ, //45
	LEX_NUM, //46
	LEX_ID, //47
	POLIZ_LABEL, //48
	POLIZ_ADDRESS, //49
	POLIZ_GO, //50
	POLIZ_FGO, //51

	/* additional lexmemes */

	LEX_BEGIN, //52
	LEX_BOOL, //53
	LEX_END, //54
	LEX_NOT, //55
	LEX_READ, //56
	LEX_THEN, //57
	LEX_TRUE, //58
	LEX_FALSE, //59
	LEX_WRITE, //60
	LEX_FIN, //61
	LEX_INT, //62
	LEX_PROGRAM //63
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
 		s << '(' << l.type_lex << ' ' << l.value_lex << ");";
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

class table_identificators{
	Identificator *p;
	int size;
	int top;
 public:
 	table_identificators(int max_size){
 		p = new Identificator[size=max_size];
 		top = 1;
 	}
 	~table_identificators(){
 		delete []p;
 	}
 	Identificator& operator[](int k){ return p[k]; }
 	int put( const char *buf );
};

int table_identificators::put(const char *buf){
	for(int j=1; j<top; ++j){
		if(!strcmp(buf,p[j].get_name()))
			return j;
	p[top].set_name(buf);
	top++;
	return top-1;
	}
	return 0;
}

class Scanner{
	
	enum state{ H , IDENT , NUMB , COM , ALE , DELIM , NEQ };
	static char *TW[];
	static type_of_lexem words[];
	static char *TD[];
	static type_of_lexem dlms[];
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

	int look(const char *buf, char **list){
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
 	Scanner (const char *program){
 		fp = fopen(program,"r");
 		current_state = H;
 		clear();
 		get_char();
 	}
 	Lexem get_lex();
};	

/* SETTING UP STATIC TABLES */

/* TABLE OF LANGUAGE KEY WORDS */
char* Scanner::TW[]={
	"", //0
	"and" //1
	"break", //2
	"case", //3
	"catch", //4
	"const", //5
	"continue" //6
	"debugger", //7
	"default", //8
	"delete", //9
	"do", //10
	"else", //11
	"finally", //12
	"for", //13
	"function", //14
	"if", //15
	"in", //16
	"instanceof", //17
	"let", //18
	"new", //19
	"or", //20
	"return", //21
	"switch", //22
	"throw", //23
	"try", //24
	"typeof", //25
	"var", //26
	"void", //27
	"while", //28
	"with", //29
};

/* TABLE OF SPECIAL SIGNS */
char* Scanner::TD[]={
	"", //0
	";", //1
	",", //2
	":", //3
	"(", //4
	")", //5
	"=", //6
	"<", //7
	">", //8
	"+", //9
	"-", //10
	"*", //11
	"/", //12
	"<=", //13
	"!=", //14
	">=", //15
};

table_identificators TID(100);

type_of_lexem
	Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BEGIN,
		LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF,
		LEX_FALSE, LEX_INT,LEX_NOT, LEX_OR,
		LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE,
		LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_NULL};


type_of_lexem
	Scanner::dlms [] = {LEX_NULL,LEX_SEMICOLON,
		LEX_COMMA, LEX_COLON,LEX_ASSIGN,LEX_LPAREN,
		LEX_RPAREN, LEX_EQ,LEX_LSS, LEX_GTR, LEX_PLUS,
		LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ,
		LEX_NEQ, LEX_GEQ, LEX_NULL};

	/* HERE LEX ANALYSATOR BASED ON GRAPH */
Lexem Scanner::get_lex(){

	int d, j;
	current_state = H;
	do
	{
		switch ( current_state ){
		case H:
			if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' ) //just skip all spaces
					get_char ();
/* ALPHA */	else if ( isalpha(c) )
			{
				clear (); //clear the buf
				add (); // add char to buf
				get_char ();
				current_state = IDENT; //alpha | digit
			}
/* DIGIT */	else if ( isdigit(c))
			{
				d = c - '0';
				get_char ();
				current_state = NUMB;
			}
/* { */     else if ( c== '{' )
			{
				get_char ();
				current_state = COM;
			}
/* : < > */	else if ( c== ':' || c== '<' || c== '>')
			{
				clear ();
				add ();
				get_char ();
				current_state = ALE;
			}
/* FIN */	else if ( c == '@' )
				return Lexem ( LEX_FIN );
/* ! */		else if ( c == '!' )
			{
				clear ();
				add ();
				get_char ();
				current_state = NEQ;
			}
			else
				current_state = DELIM;
			break;

/* END OF THE FIRST GRAPGH STAGE */			

		case IDENT:
			if ( isalpha(c) || isdigit(c) ) // while digit | alpha
			{
				add ();
				get_char ();
			}
			else if ( (j = look (buf, TW)) ) // buf == keyword 
				return Lexem (words[j], j);
			else
			{
				j = TID.put(buf); // if buf == name[j] return j, else push buf to names
				return Lexem (LEX_ID, j);
			}
		break;
		
		case NUMB:
			if ( isdigit(c) ) // while digit
			{
				d = d * 10 + (c - '0');
				get_char();
			}
			else
				return Lexem ( LEX_NUM, d );
			break;

		case COM:
			if ( c == '}' ) // until '}'
			{
				get_char ();
				current_state = H;
			}
			else if (c == '@' || c == '{' ) // ERROR
				throw c;
			else
				get_char ();
			break;

		case ALE:
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look ( buf, TD );
				return Lexem ( dlms[j], j );
			}
			else
			{
				j = look (buf, TD); // buf == sign 
				return Lexem ( dlms[j], j );
			}
			break;

		case NEQ:
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look ( buf, TD );
				return Lexem ( LEX_NEQ, j );
			}
			else //ERROR: wrong sym after !
			 throw '!';
			break;

		case DELIM:
			clear ();
			add ();
			if ( (j = look(buf, TD)) )
			{
				get_char ();
				return Lexem ( dlms[j], j );
			}
			else // ERROR
				throw c;
			break;
		
		} // end switch

	} while ( true );

}

int 
main()
{
	/* set up tables of identificators */
	table_identificators TID ( 100 ); //fills up during program's work
	const char* program = "program_path";
	Scanner scanner ( program );

	return 0;
}