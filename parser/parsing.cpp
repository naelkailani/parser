

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

AST * parse_id(j_type type) {
	SymbolTableEntry * entry=new SymbolTableEntry(ste_var,type,)
	if(!scope.insert(token))
		perror("already declaerd")
	return make_ast_node(ast_var, );
}
AST * parse_decl_list()
{
    AST * head=NULL;
    
   head= parse_decl();
    
    if( match(getCurrentToken(),lx_semicolon,"expected semicolon") )
    {
        AST * decl_list = parse_decl_list();
		head->next = decl_list;
    }
    
		return head;
}




AST * parse_decl()
{	
    AST * head=NULL;
    if( match(getCurrentToken(),kw_var) )
    {
		if (match(s.token(), lx_colon)) {
		TOKEN  * t ;
		t = parse_id();
			j_type type= parse_type();
			SymbolTableEntry * entry = new SymbolTableEntry(t->str,ste_var, type);
			if (!scope.insert(entry)) {
				perror("already exist");
			}
			head = make_ast_node(ast_var_decl, entry, type);
			return head;
		}
		else {
			perror("expected colon");
		}
    }
    else if( match(getCurrentToken(),kw_constant,"expected constant ") )
    {
		make_ast_node(ast_const_decl,)
        AST * decl_list = parse_decl_list();
		head->next = decl_list;
    }
    else if( match(getCurrentToken(),kw_function,"expected function ") )
    {
        AST * decl_list = parse_decl_list();
		head->next = decl_list;
    }
    else if( match(getCurrentToken(),kw_procedure,"expected procedure ") )
    {
        AST * decl_list = parse_decl_list();
		head->next = decl_list;
    }
	return parse_decl_list();
}

