#include <iostream>
#include <fstream>
#include <string>
#include "interpretlib.h"
using namespace std;

void Parser::dec ( type_of_lexem type ){
    int i;
    while ( !st_int.empty()){

        i = st_int.top(); 
        st_int.pop();
        if ( TID[i].get_declare() )
            throw "twice declaration";
        else
        {
            TID[i].set_declare();
            TID[i].set_type(type);
            vc_lex.push_back(type); //vector of types
        }
    }
}

void Parser::udec ()
{
    int i;
    while( !st_loc.empty() ){
        
        i = st_loc.top(); 
        st_loc.pop();
        if (i == LEX_FIN ) break; //out of the block
        if ( !TID[i].get_declare() )
            throw "try to undeclare undeclared";
        else
            TID[i].undeclare();
    }
}

void Parser::check_id(){
    if ( TID[cur_value].get_declare() )
        st_lex.push(TID[cur_value].get_type());
    else
        throw "not declared";
}

void Parser::eq_bool (){
    if ( st_lex.top() == LEX_STRING )
        throw "expression is not boolean";
    st_lex.pop();
}

/*
  need to add read(id);
void Parser::check_id_in_read (){
    if ( !TID [cur_value].get_declare() )
        throw "not declared";
}
*/