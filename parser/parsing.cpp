

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
		scanner->file->ReportError("already declaerd");
	return entry;
}
SymbolTableEntry * Parser::parse_id_routine(TOKEN * t,j_type returnType,int count) {
	SymbolTableEntry * entry;
	entry = new SymbolTableEntry(t->str, ste_routine,returnType,count);
	if (!scope.insert(t->str,entry))
		scanner->file->ReportError("already declaerd");
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
		scanner->file->ReportError("expected type");
	}
}
ast_list * Parser::parse_decl_list()
{
    ast_list * decl_list=NULL;
    
    AST * decl = parse_decl();
    if( match(getCurrentToken(),lx_semicolon) )
    {

        decl_list = parse_decl_list();
        decl_list = const_ast(decl,decl_list);
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
					AST * body = parse_block();
					scope.exitScope();
					SymbolTableEntry * entry = parse_id_routine(id, returnType, count);
					return make_ast_node(5, ast_routine_decl, entry, list_formal, returnType, body);
				}
				else {
					
					scanner->file->ReportError("expected colon");
				}
		}
		else {
			scanner->file->ReportError("expected identifier");
		}
	}
	else if(match(getCurrentToken(),kw_procedure)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
				int count = 0;
				scope.enterScope();
				ast_list * list_formal = parse_formal_list(count);
					j_type returnType =type_none;
					AST * body = parse_block();
					scope.exitScope();
					SymbolTableEntry * entry = parse_id_routine(id, returnType, count);
					return make_ast_node(5, ast_routine_decl, entry, list_formal, returnType, body);
		}
		else {
			scanner->file->ReportError("expected identifier");
		}
		
	}

	return NULL;
}
ast_list * Parser::parse_formal_list(int &count) {
	if (match(getCurrentToken(), lx_lparen)) {
		return parse_formal_list_bar(count);
	}
	else
		scanner->file->ReportError("expected left parantheses");

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
		scanner->file->ReportError("expected right  parantheses");
	}
	
}
ast_list * Parser::parse_formals(int & count) {
		if (match(getCurrentToken(), kw_var)) {
		TOKEN * id = getCurrentToken();
			if (match(id, lx_identifier)) {
				if (match(getCurrentToken(), lx_colon)) {

					j_type varType = parse_type();
					SymbolTableEntry * entry = parse_id_var(varType, id);
					AST * node = make_ast_node(3, ast_var_decl, entry, varType);
					ast_list * list = parse_formals_bar(count);
					count = 1 + count;
					return const_ast(node, list);
				}
				else {
					scanner->file->ReportError("expected colon");
				}
			}
			else {
				scanner->file->ReportError("expected identifier");
			}
		}
		else {
			scanner->file->ReportError("expected var");
		}

}
ast_list * Parser::parse_formals_bar(int &  count) {
	if (match(getCurrentToken(), lx_comma)) {

		if (match(getCurrentToken(), kw_var)) {
			TOKEN * id = getCurrentToken();
			if (match(id, lx_identifier)) {
				if (match(getCurrentToken(), lx_colon)) {

					j_type varType = parse_type();
					SymbolTableEntry * entry = parse_id_var(varType, id);
					AST * node = make_ast_node(3, ast_var_decl, entry, varType);
					ast_list * list = parse_formals_bar(count);
					count = 1 + count;
					return const_ast(node, list);
				}
				else {
					scanner->file->ReportError("expected colon");
				}
			}
			else {
				scanner->file->ReportError("expected identifer");
			}
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
				scanner->file->ReportError("expected end");
			}
		}
    return NULL;
}
ast_list * Parser::parse_var_decl_list()
{
    ast_list * var_decl_list=NULL;
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
				scanner->file->ReportError("expected colon");
			}

		}
		else {
			scanner->file->ReportError("expected id");
		}
	}
	else if(match(getCurrentToken(),kw_constant)){
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			if (match(getCurrentToken(), lx_eq)) {


				AST * exp=parse_expr();
				int value=eval_ast_expr(scanner->file, exp);
				SymbolTableEntry * entry=parse_id_cons(id,value);
				return make_ast_node(3,ast_const_decl,entry,value);
			}
			else {
				scanner->file->ReportError("expected =");
			}

		}
		else
		{
			scanner->file->ReportError("expected id");
		}
	}
	return  NULL;
}
ast_list * Parser::parse_stmt_list()
{
    ast_list * stmt_list=NULL;
    
    AST * stmt = parse_stmt();

    if(match(getCurrentToken(),lx_semicolon) )
    {
        stmt_list = parse_stmt_list();
        stmt_list = const_ast(stmt,stmt_list);
    }
	//lamda
    return stmt_list;
}
AST * Parser::parse_call_assgin(TOKEN * id) {
		// assign
		if (match(getCurrentToken(), lx_colon_eq)) {
			AST * exp = parse_expr();
			SymbolTableEntry * e = scope.getFirstOcc(id->str);
			if (e->entry_type == ste_undefined) {
				scanner->file->ReportError("undefined variable ");
			}
			else {
				return make_ast_node(3, ast_assign, e, exp);
			}
			}
		// call
		else {
			int count = 0;
			ast_list * args = parse_arg_list(count);
			SymbolTableEntry * e = scope.getFirstOcc(id->str);
			if (e->entry_type == ste_undefined) {
				scanner->file->ReportError("undefined function ");
			}
			else if (count != e->f.routine.formalNumber) {
				scanner->file->ReportError("wrong number of arguments");
			}
			else {
				return make_ast_node(3,ast_call, e, args);
			}
		}
}
AST * Parser::parse_ifstmt() {
		AST * exp = parse_expr();
		if (match(getCurrentToken(), kw_then)) {
			AST * consequance = parse_stmt();
			if (match(getCurrentToken(), lx_semicolon)) {
				if (match(getCurrentToken(), kw_fi)) {
					return make_ast_node(4, ast_if, exp, consequance, NULL);
				}
				// ifstmt_bar
				else if (match(getCurrentToken(), kw_else)) {
					AST * alt = parse_stmt();
					if (match(getCurrentToken(), lx_semicolon)) {
						if (match(getCurrentToken(), kw_fi)) {
							return make_ast_node(4, ast_if, exp, consequance, alt);
						}
						else {

							scanner->file->ReportError("expected else or fi");
						}
					}
					else {

						scanner->file->ReportError("expected ;");
					}
				}
				else {
					scanner->file->ReportError("expected else or fi");
				}
			}
			else {

				scanner->file->ReportError("expected ;");
			}
		}
		else {
			scanner->file->ReportError("expected then");
		}
}
AST * Parser::parse_stmt() {
	TOKEN * id = getCurrentToken();
	if (match(getCurrentToken(), lx_identifier)) {
		return parse_call_assgin(id);
	}
	else if (match(getCurrentToken(), kw_if)) {
		return parse_ifstmt();
	}
	else if (match(getCurrentToken(), kw_while)) {
		AST * predeict = parse_expr();
		if (match(getCurrentToken(), kw_do)) {
			AST * body = parse_stmt();
			if (match(getCurrentToken(), lx_semicolon)) {
				if (match(getCurrentToken(), kw_od)) {
					return make_ast_node(4, ast_while, predeict, body);
				}
				else {

					scanner->file->ReportError("expected od");
				}
			}
			else {
				scanner->file->ReportError("expected ;");
			}
		}
		else {

				scanner->file->ReportError("expected do");
			}
	
	}
	else if (match(getCurrentToken(), kw_for)) {
		TOKEN * id = getCurrentToken();
		if (match(id, lx_identifier)) {
			AST * assign = parse_call_assgin(id);
			SymbolTableEntry * var;
			if (assign->type == ast_call) {
				var = assign->f.a_call.callee;
			}
			else {

			}
			AST * lower = assign;
			if (match(getCurrentToken(), kw_to)) {
				AST * upper = parse_expr();
				if (match(getCurrentToken(), kw_do)) {
					ast_list * body = parse_stmt_list();
					if (match(getCurrentToken(), kw_od))
						return make_ast_node(5, ast_for,lower->f.a_assign.lhs, lower, upper, body);
					else {
						scanner->file->ReportError("expected od");
					}
				}
				else {
					scanner->file->ReportError("expected do");
				}
			}
			else {
				scanner->file->ReportError("expected to");
			}
		}
		else {
			scanner->file->ReportError("expected an identifier");
		}
	}
	else if (match(getCurrentToken(), kw_read)) {
		if (match(getCurrentToken(), lx_lparen)) {
			TOKEN * id = getCurrentToken();
			if (match(id, lx_identifier)) {
				SymbolTableEntry * entry = scope.getFirstOcc(id->str);
				if (entry->entry_type == ste_undefined) {
					scanner->file->ReportError("undefined variable");
				}
				if (match(getCurrentToken(), lx_rparen)) {
					return make_ast_node(2, ast_read, entry);
				}
				else {
					scanner->file->ReportError("expect right parantheses");
				}
			}
			else {
				scanner->file->ReportError("expect an indentifier");
			}
		}
		else {
			scanner->file->ReportError("expect a left parantheses");
		}
	}
	else if (match(getCurrentToken(), kw_write)) {
		if (match(getCurrentToken(), lx_lparen)) {
			TOKEN * id = getCurrentToken();
			if (match(id, lx_identifier)) {
				SymbolTableEntry * entry = scope.getFirstOcc(id->str);
				if (entry->entry_type == ste_undefined) {
					scanner->file->ReportError("undefined variable");
				}
				if (match(getCurrentToken(), lx_rparen)) {
					return make_ast_node(2, ast_write, entry);
				}
				else {
					scanner->file->ReportError("expect right parantheses");
				}
			}
			else {
				scanner->file->ReportError("expect an indentifier");
			}
		}
		else {
			scanner->file->ReportError("expect a left parantheses");
		}
	}
	else if (match(getCurrentToken(), kw_return)) {
		if (match(getCurrentToken(), lx_lparen)) {
			AST * exp = parse_expr();
			if (match(getCurrentToken(), lx_rparen)) {
				return make_ast_node(2,ast_return, exp);
			}
			else {

			scanner->file->ReportError("expect right parantheses");
			}
		}
		else {
			scanner->file->ReportError("expect left parantheses");
		}
	}
	else if(getCurrentToken()->type==kw_begin){
		return parse_block();
	}
	return NULL;
}

ast_list * Parser::parse_arg_list(int &count) {
	if (match(getCurrentToken(), lx_lparen)) {
		return parse_arg_list_bar(count);
	}
	else
		//scanner->file->ReportError("expcted left parantheses");
		return NULL;

}
ast_list * Parser::parse_arg_list_bar(int & count) {
	if(match(getCurrentToken(),lx_rparen)){
		return NULL;
	}
	ast_list * list=parse_args(count);
	
	if(match(getCurrentToken(),lx_rparen)){
		return list;
	}
	else {
		scanner->file->ReportError("expected left para");
	}
	
}
ast_list * Parser::parse_args(int & count) {
	AST * exp = parse_expr();
	ast_list * list = parse_args_bar(count);
	count = 1 + count;
	return const_ast(exp, list);

}
ast_list * Parser::parse_args_bar(int &  count) {
	if (match(getCurrentToken(), lx_comma)) {
		AST * exp = parse_expr();
		ast_list * list = parse_args_bar(count);
		count = 1 + count;
		return const_ast(exp, list);
	}
	return NULL;
}

AST * Parser::parse_expr() {
	AST * r = parse_rel();
	return parse_expr_bar(r);
}
AST * Parser::parse_expr_bar(AST * expr_l) {
	AST * expr_r;
	AST * expr;
	if (match(getCurrentToken(), kw_and)) {
		expr_r = parse_rel();
		expr = make_ast_node(3, ast_and, expr_l, expr_r);
		return parse_expr_bar(expr);
	}
	else if (match(getCurrentToken(), kw_or)) {
		expr_r = parse_rel();
		expr = make_ast_node(3, ast_or, expr_l, expr_r);
		return parse_expr_bar(expr);
	}
	return expr_l;
}
AST * Parser::parse_rel() {
	AST * arith = parse_arith();
	return parse_rel_bar(arith);
}
AST * Parser::parse_rel_bar(AST * rel_l) {
	TOKEN * t = getCurrentToken();
	AST * rel_r;
	AST * rel=NULL;
	if (match(t, lx_le)) {
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_le, rel_l, rel_r);
		return parse_rel_bar(rel);
	}
	else if( match(t, lx_gt) ){
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_gt, rel_l, rel_r);
		return parse_rel_bar(rel);
		}
	else if( match(t, lx_ge) ){
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_ge, rel_l, rel_r);
		return parse_rel_bar(rel);
		}
	else if( match(t, lx_lt)){
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_lt, rel_l, rel_r);
		return parse_rel_bar(rel);
		}
	else if(match(t,lx_eq)){
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_eq, rel_l, rel_r);
		return parse_rel_bar(rel);
		}
	else if (match(t, lx_neq)) {
		rel_r = parse_arith();
		rel=make_ast_node(3, ast_neq, rel_l, rel_r);
		return parse_rel_bar(rel);
	}
		return rel_l;
}
AST * Parser::parse_arith() {
	AST * unary = parse_unary();
	return parse_arith_bar(unary);
}
AST * Parser::parse_arith_bar(AST * arith_l) {
	TOKEN * t = getCurrentToken();
	AST * arith_r;
	AST * arith;
	if (match(t, lx_plus)) {
		arith_r = parse_unary();
		arith=make_ast_node(3, ast_plus, arith_l,arith_r);
		return parse_arith_bar(arith);
	}
	else if(match(t, lx_minus) ){
		arith_r = parse_unary();
		arith=make_ast_node(3, ast_minus, arith_l,arith_r);
		return parse_arith_bar(arith);
	}
	else if (match(t, lx_star)) {
		arith_r = parse_unary();
		arith=make_ast_node(3, ast_times, arith_l,arith_r);
		return parse_arith_bar(arith);
	}
	else if (match(t, lx_slash)) {
		arith_r = parse_unary();
		arith=make_ast_node(3, ast_divide, arith_l,arith_r);
		return parse_arith_bar(arith);
	}

		return arith_l;
}
AST * Parser::parse_unary(){
	AST_type type;
	AST * f;
	if (match(getCurrentToken(), kw_not)) {
		type = ast_not;
		f = parse_f();
		return make_ast_node(2, type, f);
	}
	else if( match(getCurrentToken(), lx_minus)) {
		type = ast_uminus;
		f = parse_f();
		return make_ast_node(2, type, f);
	}
	return parse_f();
}
AST * Parser::parse_call_id(TOKEN * id) {


	if (getCurrentToken()->type==lx_lparen) {
		int count = 0;
		ast_list * args = parse_arg_list(count);
		SymbolTableEntry * e = scope.getFirstOcc(id->str);
		if (e->entry_type == ste_undefined) {
			scanner->file->ReportError("undefined function ");
		}
		else if (count != e->f.routine.formalNumber) {
			scanner->file->ReportError("wrong number of arguments");
		}
		else {
			return make_ast_node(3, ast_call, e, args);
		}
	}
		else {
			SymbolTableEntry * e = scope.getFirstOcc(id->str);
			if (e->entry_type == ste_undefined) {
				scanner->file->ReportError("undefined variable ");
			}
			else {
				return make_ast_node(2, ast_var, e);
			}
			}
}
AST * Parser::parse_f() {
	TOKEN * t = getCurrentToken();
	if (match(t, lx_identifier)) {
		return parse_call_id(t);
	}
	else if (match(getCurrentToken(), lx_integer)) {
		return make_ast_node(2,ast_integer, t->value);
	}
	else if (match(getCurrentToken(), lx_string)) {
		return make_ast_node(2,ast_string, t->str);
	}
	else if (match(getCurrentToken(), kw_true)) {

		return make_ast_node(2,ast_boolean, 1);
	}
	else if (match(getCurrentToken(), kw_false)) {

		return make_ast_node(2,ast_boolean, 0);
	}
	else if (match(getCurrentToken(), lx_lparen)) {
		AST * expr = parse_expr();
		if (match(getCurrentToken(), lx_rparen)) {
			return expr;
		}
		else {
			scanner->file->ReportError("expected right paranthese");
		}
	}
	else {
		scanner->file->ReportError("invalid expression");
	}
}
