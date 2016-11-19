

#include "parsing.hpp"
#include "ast.h"
#include "scanner.h"
#include <iostream>
#include<string>

using namespace std;

TOKEN * current;


TOKEN * getCurrentToken()
{

    return new TOKEN();
}

TOKEN * getNextToken()
{
    
    
    return new TOKEN();
}

AST * parse_program()
{
    
    AST* o =  parse_decl_list();
    return o;
    
}


bool match(TOKEN * token,LEXEME_TYPE lexemeType,string msg)
{
    if(token->type == lexemeType)
    {
        current = getNextToken();
        return true;
    }
    
    
    
    
    cout<<msg;
    return false;
}


AST * parse_decl_list()
{
    AST * decl_list_ast;
    
    decl_list_ast = parse_decl();
    
    if( match(getCurrentToken(),lx_semicolon,"expected semicolon") )
    {
        
        decl_list_ast = parse_decl_list();
    }
    else
    {
        
        throw new exception();
    }
    
    return decl_list_ast;
    
    
}



AST * parse_decl()
{
    
    return new AST();
}

