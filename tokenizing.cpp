#include <iostream>
#include <fstream>
#include <string.h>
#include <exception>
#include <stdlib.h>
#include <string>
#include <vector>
#include "interpretlib.h"

using namespace std;

#define PROG_PATH "./input_file" 

/* ALL! javascript types of lexem (some may be useless) */
<<<<<<< HEAD
=======
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
	//LEX_ASSIGN,
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
	LEX_CHAR, //57
    LEX_TRUE, //58
    LEX_FALSE //59
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

>>>>>>> 066d9094d4bf2a37d91b331b4c7b5e42593635d9
class table_identificators{
	Identificator *p;
	int size;
	int top;
 public:
 	table_identificators(int max_size){
 		p = new Identificator[size=max_size];
 		//p[0]= Identificator();
 		top = 1;
 	}
 	~table_identificators(){
 		delete []p;
 	}
 	Identificator& operator[](int k){ return p[k]; }
 	int put( const char *buf );
};

int table_identificators::put(const char *buf){
	for(int j=1; j<top; j++)
		if(!strcmp(buf,p[j].get_name()))
			return j;
	p[top].set_name(buf);
	top++;
	return top-1;
}

<<<<<<< HEAD
=======
class Scanner{
	
	enum state { H , IDENT , NUMB , COM1 , COM2 , ALE , COMP , DELIM , NEQ , STRING , ERROR};
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
>>>>>>> 066d9094d4bf2a37d91b331b4c7b5e42593635d9


/* SETTING UP STATIC TABLES */

/* TABLE OF LANGUAGE KEY WORDS */
const char* Scanner::TW[]={
	"", //0
	"and", //1
	"break", //2
	"case", //3
	"catch", //4
	"const", //5
	"continue", //6
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
    "true", //30
    "false",//31
	 NULL
};

/* TABLE OF SPECIAL SIGNS */
const char* Scanner::TD[]={
	"", //0
	";", //1
	",", //2
	"{", //3
	"}", //4
	":", //5
	"(", //6
	")", //7
	"=", //8
	"<", //9
	">", //10
	"+", //11
	"-", //12
	"*", //13
	"/", //14
	"<=", //15
	"!=", //16
	">=", //17
    "==", //18
    "===",//19
	//"\"", //20
	//"'", //21

	"@", //22

	 NULL
};

table_identificators TID(100);


type_of_lexem
	Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BREAK,
		LEX_CASE, LEX_CATCH, LEX_CONST, LEX_CONTINUE, 
		LEX_DEBUGGER, LEX_DEFAULT, LEX_DELETE, LEX_DO, LEX_ELSE,
		LEX_FINALLY, LEX_FOR, LEX_FUNCTION, LEX_IF, LEX_IN, 
		LEX_INSTANCEOF, LEX_LET, LEX_NEW, LEX_OR, LEX_RETURN,
		LEX_SWITCH, LEX_THROW, LEX_TRY, LEX_TYPEOF, LEX_VAR, 
		LEX_VOID, LEX_WHILE, LEX_WITH, LEX_TRUE, LEX_FALSE, LEX_NULL};


type_of_lexem

	Scanner::dlms [] = {LEX_NULL, LEX_SEMICOLON, LEX_COMMA, //LEX_ASSIGN, 
		LEX_LBRACE, LEX_RBRACE, LEX_COLON, 
		LEX_LPAREN, LEX_RPAREN, LEX_EQ,LEX_LSS, LEX_GTR, 
		LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ,
<<<<<<< HEAD
		LEX_NEQ, LEX_GEQ,LEX_DEQ,LEX_TEQ,LEX_STRING,LEX_FIN, LEX_NULL};
=======
		LEX_NEQ, LEX_GEQ, LEX_DEQ, LEX_TEQ, LEX_STRING, LEX_CHAR, LEX_FIN, LEX_NULL};
>>>>>>> 066d9094d4bf2a37d91b331b4c7b5e42593635d9


	/* HERE LEX ANALYSATOR BASED ON GRAPH */

vector<string> TS;  // table of strings

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

            

			/* HERE NEED TO INSERT string processing "..." and '.'*/
			else if (c== '\"') //|| c== '\'') 
			{
				/* After spaces only works*/

				get_char();	
				current_state = STRING; //need to add new state 
			}
			/*else if(c== '\''){
				get_char();
				current_state = CHAR;
			}*/

/* COM1 */  else if ( c== '\\')
			{
				get_char ();
				if (c== '\\')
					current_state = COM1;
				else throw c;
			}
/* COM2 */ 	else if ( c== '/')
			{
				get_char();
				if( c== '*')
					current_state = COM2;
				else throw c;
			}
/* = < > */	else if ( c== '=')
			{
				clear ();
				add ();
				get_char ();
				current_state = ALE;
			}
			else if(c== '<' || c== '>'){
				clear ();
				add ();
				get_char ();
				current_state = COMP;
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
			if(c == '\"' || c=='\''){
				clear();
				current_state = ERROR;
			}

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

		case COM1: // ...
			if ( c == '\n') // until end of line 
			{
				get_char ();
				current_state = H;
			}
			else if (c == '@') // ERROR
				current_state=ERROR;
			else
				get_char ();
			break;
		case COM2: /* ... */
			if (c == '*')
			{
				get_char ();
				if (c == '/')
				{
					get_char();
					current_state = H;
				}
			}
			else if (c == '@') //ERROR

				current_state = ERROR;

			else get_char();
			break;

		case ALE:
			if ( c == '=' )
			{
				add ();
				get_char ();
				if(c == '='){
					add();
					get_char();
					j = look ( buf, TD );
					return Lexem ( dlms[j], j );
				}
				else{
					j = look ( buf, TD );
					return Lexem ( dlms[j], j );
				}
			}
			else
			{
				j = look (buf, TD); // buf == sign 
				return Lexem ( dlms[j], j );
			}
			break;
		case COMP:{
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look (buf, TD); // buf == sign 
				return Lexem ( dlms[j], j );
			}
            else 
            {
				j = look (buf, TD); // buf == sign 
				return Lexem ( dlms[j], j );
            }
			break;

		}

		case NEQ:
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look ( buf, TD );
				return Lexem ( LEX_NEQ, j );
			}
			else //ERROR: not !=

				current_state =ERROR;

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

				current_state = ERROR;
			break;
		 
/*STRING*/
		case STRING:{
			
			if(c=='\"'){
				string str(buf+1);
				add();
				get_char();
				TS.push_back(str);
				return Lexem(LEX_STRING,TS.size());
			}
			else if(c=='@'){
				clear();
				current_state = ERROR;
			}
			else{
				add();
				get_char();
			}
			break;
		}


		case ERROR:{
			throw c;
			break;
		}
	} // end of switch


	} while ( true );

<<<<<<< HEAD
}
=======
}

int main()

{
	/* set up tables of identificators */
	ofstream out ("output_file");
 try
 {
	const char* program = PROG_PATH;
	Scanner scanner ( program );
	Lexem lex;
	int k = 0;
	while (lex.get_type() != LEX_FIN)
	{
		if ( ++k % 10 == 0) out << endl;
		lex = scanner.get_lex();
		out << lex;
	}	
 }

catch(invalid_argument& err)
	{
		cerr << "Exception catched : " << err.what() << endl;
	}
catch(char& c)
	{
		cerr << "Exception catched : lex error : " << c << endl;
	}
	out << endl << endl;
	int i=1;
	vector <string>::const_iterator p = TS.begin();
 	while(p!=TS.end()){
 		out << i <<" "<< *p << endl;
 		p++;
 		i++;
 	}
	return 0;
}
>>>>>>> 066d9094d4bf2a37d91b331b4c7b5e42593635d9
