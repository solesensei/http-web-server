#include <iostream>
#include <fstream>
#include <string>
#include "interpretlib.h"
using namespace std;

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

void Parser::check_op (){
    type_of_lexem t1, t2, op, t = LEX_VAR, r = LEX_BOOL;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if ( op == LEX_OR || op == LEX_AND )
         t = LEX_BOOL;
    else //all others operation
         r = LEX_VAR;
    if ( t1 == t2 && t1 == t )
        st_lex.push(r);
    else
        throw "wrong types are in operation";
}

void Parser::eq_type (){
    if ( st_lex.pop() != st_lex.pop() ) throw "wrong types are in =";
}

/* how is LEX_BOOL working ?
void Parser::eq_bool (){
    if ( st_lex.pop() != LEX_BOOL )
        throw "expression is not boolean";
}
*/

/* need to add read(id);
void Parser::check_id_in_read (){
    if ( !TID [cur_value].get_declare() )
        throw "not declared";
}
*/