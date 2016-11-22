

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


SymbolTableEntry * Parser::parse_id_var(j_type type,TOKEN * t) {
	SymbolTableEntry * entry;
		entry = new SymbolTableEntry(t->str, ste_var, type);
	if (!scope.insert(t->str,entry))
		perror("already declaerd");
	return entry;
}
SymbolTableEntry * Parser::parse_id_cons(TOKEN * t,int value) {
	SymbolTableEntry * entry;
	entry = new SymbolTableEntry(t->str, ste_const, value);
	if (!scope.insert(t->str,entry))
		perror("already declaerd");
	return entry;
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

j_type parse_type() {
	TOKEN * t = getCurrentToken();
	if (match(t, kw_integer)) {
		return type_integer;
	}
	else if (match(t, kw_string)) {
		return type_string;
	}
	else if (match(t, kw_boolean)) {
		return type_boolean;
	}
	else {
		throw new exception();
	}
}

AST * Parser::parse_decl()
{
  	if(match(getCurrentToken(),kw_var)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType=parse_type();
				SymbolTableEntry * entry=parse_id_var(varType,id);
				return make_ast_node(ast_var_decl,entry,varType);
			}

		}
	}
	else if(match(getCurrentToken(),kw_constant)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_eq)) {


				AST * exp=parse_exp();
				int value=eval_ast_expr(fp, exp);
				SymbolTableEntry * entry=parse_id_cons(id,value);
				return make_ast_node(ast_const_decl,entry,value);
			}

		}
	}
	else if(match(getCurrentToken(),kw_function)){
		
	}
	else if(match(getCurrentToken(),kw_procedure)){
		
	}
	else{

	}

    return new AST();

}
