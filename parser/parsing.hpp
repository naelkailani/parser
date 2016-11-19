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
#include <iostream>
#include<string>


TOKEN * getCurrentToken();
TOKEN * getNextToken();
bool match(TOKEN * token,LEXEME_TYPE lexemeType,string msg);

AST * parse_program();
AST * parse_decl_list();
AST * parse_decl();

#endif /* parsing_hpp */
