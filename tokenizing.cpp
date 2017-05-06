#include <iostream>
#include <fstream>
#include <string.h>
#include <exception>
#include <stdlib.h>
#include <string>
#include <vector>
#include "interpretlib.h"

int table_identificators::put(const char *buf){
	for(int j=1; j<top; j++)
		if(!strcmp(buf,p[j].get_name()))
			return j;
	p[top].set_name(buf);
	top++;
	return top-1;
}
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
	"bool",//32
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

vector<string> TS;

type_of_lexem
	Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BREAK,
		LEX_CASE, LEX_CATCH, LEX_CONST, LEX_CONTINUE, 
		LEX_DEBUGGER, LEX_DEFAULT, LEX_DELETE, LEX_DO, LEX_ELSE,
		LEX_FINALLY, LEX_FOR, LEX_FUNCTION, LEX_IF, LEX_IN, 
		LEX_INSTANCEOF, LEX_LET, LEX_NEW, LEX_OR, LEX_RETURN,
		LEX_SWITCH, LEX_THROW, LEX_TRY, LEX_TYPEOF, LEX_VAR, 
		LEX_VOID, LEX_WHILE, LEX_WITH, LEX_TRUE, LEX_FALSE, LEX_BOOL, LEX_NULL};


type_of_lexem

	Scanner::dlms [] = {LEX_NULL, LEX_SEMICOLON, LEX_COMMA, //LEX_ASSIGN, 
		LEX_LBRACE, LEX_RBRACE, LEX_COLON, 
		LEX_LPAREN, LEX_RPAREN, LEX_EQ,LEX_LSS, LEX_GTR, 
		LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ,
		LEX_NEQ, LEX_GEQ, LEX_DEQ, LEX_TEQ, LEX_STRING, LEX_FIN, LEX_NULL};


	/* HERE LEX ANALYSATOR BASED ON GRAPH */
Lexem Scanner::get_lex(int *str_num){

	int d, j;
	current_state = H;
	do
	{
	switch ( current_state ){
		case H:
			if ( c ==' ' || c=='\r' || c =='\t' ) //just skip all spaces
				get_char ();
			else if(c=='\n'){
				(*str_num)++;
				get_char();
			}
/* ALPHA */	else if (isalpha(c))
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
/* FIN */	else if ( c == '@' ){
				return Lexem (LEX_FIN,0);
			}
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
				return Lexem ( words[j],j);
			else
			{
				j = TID.put(buf); // if buf == name[j] return j, else push buf to names
				return Lexem (LEX_ID,j);
			}
		break;
		
		case NUMB:
			if ( isdigit(c) ) // while digit
			{
				d = d * 10 + (c - '0');
				get_char();
			}
			else
				return Lexem (LEX_NUM,d );
			break;

		case COM1: // ...
			if ( c == '\n') // until end of line 
			{
				(*str_num)++;
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
					return Lexem (dlms[j],j );
				}
				else{
					j = look ( buf, TD );
					return Lexem (dlms[j], j );
				}
			}
			else
			{
				j = look (buf, TD); // buf == sign 
				return Lexem (dlms[j],j );
			}
			break;
		case COMP:{
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look (buf, TD); // buf == sign 
				return Lexem (dlms[j], j );
			}
            else 
            {
				j = look (buf, TD); // buf == sign 
				return Lexem (dlms[j], j );
            }
			break;

		}

		case NEQ:
			if ( c == '=' )
			{
				add ();
				get_char ();
				j = look ( buf, TD );
				return Lexem (LEX_NEQ,j );
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
				return Lexem (dlms[j], j );
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
}
