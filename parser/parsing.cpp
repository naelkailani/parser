

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
    
//    AST* o =  parse_decl_list();
    return o;
    
}


bool match(TOKEN * token,LEXEME_TYPE lexemeType)
{
    if(token->type == lexemeType)
    {
        current = getNextToken();
        return true;
    }
    
    return false;
}


ast_list * parse_decl_list()
{
    ast_list * decl_list;
    
    AST * decl = parse_decl();
    
    if( match(getCurrentToken(),lx_semicolon) )
    {
        decl_list = parse_decl_list();
        decl_list = const_ast(decl,decl_list);
    }
    else
    {
        
        throw new exception();
    }
    
    return decl_list;
    
    
}

SymbolTableEntry * parseId(){
	TOKEN * token=getCurrentToken();
	
}

AST * parse_decl()
{
  	if(match(getCurrentToken(),kw_var)){
			
	}
	else if(match(getCurrentToken(),kw_constant)){
		
	}
	else if(match(getCurrentToken(),kw_function)){
		
	}
	else if(match(getCurrentToken(),kw_procedure)){
		
	}
	else{

	}

    return new AST();

}

