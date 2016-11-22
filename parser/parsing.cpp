

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
SymbolTableEntry * Parser::parse_id_routine(TOKEN * t,j_type returnType,int count) {
	SymbolTableEntry * entry;
	entry = new SymbolTableEntry(t->str, ste_routine,returnType,count);
	if (!scope.insert(t->str,entry))
		perror("already declaerd");
	return entry;
}
ast_list * Parser::parse_decl_list()
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

j_type Parser::parse_type() {
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
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
				int count = 0;
				scope.enterScope();
				ast_list * list_formal = parse_formal_list(count);
				if (match(getCurrentToken(), lx_colon)) {
					j_type returnType = parse_type();
					SymbolTableEntry * entry = parse_id_routine(id, returnType, count);
					AST * body = parse_block();
					scope.exitScope();
					return make_ast_node(ast_routine_decl, entry, list_formal, returnType, body);
				}
				else {
					throw new exception();
				}
		}
		else {
			throw new exception();
		}
	}
	else if(match(getCurrentToken(),kw_procedure)){
			
	}
	else{

	}

    return new AST();

}
ast_list * Parser::parse_formal_list(int &count) {
	if (match(getCurrentToken(), lx_lparen)) {
		return parse_formal_list_bar(count);
	}
	else
		throw new exception();

}
ast_list * Parser::parse_formal_list_bar(int & count) {
	if(match(getCurrentToken(),lx_rparen)){
		return NULL;
	}
	return  parse_formals(count);
	
	
}
ast_list * Parser::parse_formals(int & count) {
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType = parse_type();
				SymbolTableEntry * entry = parse_id_var(varType, id);
				AST * node = make_ast_node(ast_var_decl, entry, varType);
				ast_list * list = parse_formals_bar(count);
				count = 1 + count;
				return const_ast(node, list);
			}
			else {
				throw new exception();
			}
		}
		else {
			throw new exception();
		}

}
ast_list * Parser::parse_formals_bar(int &  count) {
	if (match(getCurrentToken(), lx_comma)) {

		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType = parse_type();
				SymbolTableEntry * entry = parse_id_var(varType, id);
				AST * node = make_ast_node(ast_var_decl, entry, varType);
				ast_list * list = parse_formals_bar(count);
				count = 1 + count;
				return const_ast(node, list);
			}
			else {
				throw new exception();
			}
		}
		else {
			return NULL;
		}
	}
}
AST * Parser::parse_block(){
		if (match(getCurrentToken(), kw_begin)) {

		}
		else {
		}
		return NULL;
}

ast_list * Parser::parse_var_decl_list()
{
    ast_list * var_decl_list;
    
    AST * var_decl = parse_var_decl();
    if( match(getCurrentToken(),lx_semicolon) )
    {
        var_decl_list = parse_var_decl_list();
        var_decl_list = const_ast(var_decl,var_decl_list);
    }
    else
    {
        
        throw new exception();
    }
    
    return var_decl_list;
}
AST * Parser::parse_var_decl() {
  	if(match(getCurrentToken(),kw_var)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType=parse_type();
				SymbolTableEntry * entry=parse_id_var(varType,id);
				return make_ast_node(ast_var_decl,entry,varType);
			}
			else {
				throw new exception();
			}

		}
		else {
			throw new exception();
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
			else {
				throw new exception();
			}

		}
		else
		{
			throw new exception();
		}
	}
	else {
		throw new exception();
	}
}
