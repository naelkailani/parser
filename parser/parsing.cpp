

#include "scanner.h"
#include "ast.h"
#include "parsing.hpp"
#include <iostream>
#include<string>

using namespace std;



TOKEN * Parser::getCurrentToken()
{
	return scanner->Token();
}

TOKEN * Parser::getNextToken()
{
    
    
    return new TOKEN();
}

AST * Parser::parse_program()
{
    
    //AST* o =  parse_decl_list();
    return NULL;
    
}

bool Parser::match(TOKEN * token, LEXEME_TYPE lexemeType){
	TOKEN * current = NULL;
    if(token->type == lexemeType)
    {
		scanner->nextToken();
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
AST * Parser::parse_decl()
{
	AST * dec=parse_var_decl();
	if (dec != NULL)return dec;
	 if(match(getCurrentToken(),kw_function)){
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
					return make_ast_node(5, ast_routine_decl, entry, list_formal, returnType, body);
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
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
				int count = 0;
				scope.enterScope();
				ast_list * list_formal = parse_formal_list(count);
					j_type returnType =type_none;
					SymbolTableEntry * entry = parse_id_routine(id, returnType, count);
					AST * body = parse_block();
					scope.exitScope();
					return make_ast_node(5, ast_routine_decl, entry, list_formal, returnType, body);
		}
		else {
			throw new exception();
		}
		
	}
	else{

			throw new exception();
	}

	return NULL;
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
	ast_list * list=parse_formals(count);
	
	if(match(getCurrentToken(),lx_rparen)){
		return list;
	}
	else {
		throw new exception();
	}
	
}
ast_list * Parser::parse_formals(int & count) {
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType = parse_type();
				SymbolTableEntry * entry = parse_id_var(varType, id);
				AST * node = make_ast_node(3,ast_var_decl, entry, varType);
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
				AST * node = make_ast_node(3,ast_var_decl, entry, varType);
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
    return NULL;
}
AST * Parser::parse_block(){
		if (match(getCurrentToken(), kw_begin)) {

			ast_list * var_list = parse_var_decl_list();
			ast_list * stmt_list = parse_stmt_list();
			if (match(getCurrentToken(), kw_end)) {
				return make_ast_node(3,ast_block, var_list, stmt_list);
			}
			else {
				throw new exception();
			}
		}
		else {
			throw new exception();
		}
    return NULL;
}
ast_list * Parser::parse_var_decl_list()
{
    ast_list * var_decl_list;
    AST * var_decl = parse_var_decl();
	if (var_decl == NULL)return NULL;
    if( match(getCurrentToken(),lx_semicolon) )
    {
        var_decl_list = parse_var_decl_list();
        var_decl_list = const_ast(var_decl,var_decl_list);
    }
	// lamda    
    return var_decl_list;
}
AST * Parser::parse_var_decl() {
  	if(match(getCurrentToken(),kw_var)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_colon)) {

				j_type varType=parse_type();
				SymbolTableEntry * entry=parse_id_var(varType,id);
				return make_ast_node(3,ast_var_decl,entry,varType);
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


				//AST * exp=parse_expr();
				//int value=eval_ast_expr(fp, exp);
				//SymbolTableEntry * entry=parse_id_cons(id,value);
				//return make_ast_node(ast_const_decl,entry,value);
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
	return  NULL;
}
ast_list * Parser::stmt_list()
{
    ast_list * stmt_list;
    
    AST * stmt = parse_stmt();

    if(match(getCurrentToken(),lx_semicolon) )
    {
        stmt_list = parse_stmt_list();
        stmt_list = const_ast(stmt,stmt_list);
    }
	//lamda
    return stmt_list;
}
AST * Parser::parse_stmt() {
	AST * stmt = NULL;
	stmt = parse_call_assign();
	if (stmt != NULL)return;
	stmt = parse_ifstmt();
	if (stmt != NULL)return;
	if (match(getCurrentToken(), kw_while)) {
		AST * predeict = parse_expr();
		if (match(getCurrentToken(), kw_do)) {
			AST * body= parse_stmt();
			if (match(getCurrentToken(), kw_od))
				return make_ast_node(4, ast_while, predeict, body);
			throw new exception();
		}
		else {
			throw new exception();
		}
	}
	else if (match(getCurrentToken(), kw_for)) {
		AST * assign = parse_assign();
		SymbolTableEntry var = assign->f.a_for.var;
		if (match(getCurrentToken(), kw_to)) {
			AST * upper = parse_expr();
			if (match(getCurrentToken(), kw_do)) {
				AST * body = parse_stmt();
				if (match(getCurrentToken(), kw_od))
					return make_ast_node(ast_for,);
			}
		}
	}
}
