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
	AST * parse_decl();
	bool match(TOKEN * token, LEXEME_TYPE lexemeType);

	AST * parse_program();
	AST * parse_decl_list();
	AST * parse_decl();
};

#endif /* parsing_hpp */
