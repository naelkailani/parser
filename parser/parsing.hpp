//
//  parsing.hpp
//  parser
//
//  Created by Nael Kilani on 11/19/16.
//  Copyright © 2016 Nael Kilani. All rights reserved.
//

#ifndef parsing_hpp
#define parsing_hpp

#include <stdio.h>
#include "ast.h"
#include "scanner.h"
#include"symbol.h"
#include <iostream>
#include<string>

class Parser {
public:
	Scope scope;
	SCANNER scanner;
	
	TOKEN * getCurrentToken();
	TOKEN * getNextToken();
	SymbolTableEntry * parse_id_var(j_type type, TOKEN * t);
	SymbolTableEntry * parse_id_cons(TOKEN * t, int value);
	SymbolTableEntry * parse_id_routine(TOKEN * t, j_type returnType, int count);
	j_type parse_type();
	AST * parse_decl();
	ast_list * parse_formal_list(int &);
	ast_list * parse_formal_list_bar(int &);
	ast_list * Parser::parse_formals_bar(int &);
	ast_list * parse_formals(int &);
	bool match(TOKEN * token, LEXEME_TYPE lexemeType);

	AST * parse_program();
	ast_list * parse_decl_list();
};

#endif /* parsing_hpp */
