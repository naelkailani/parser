//
//  main.cpp
//  parser
//
//  Created by Nael Kilani on 11/18/16.
//  Copyright © 2016 Nael Kilani. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include"symbol.h"
#include <unordered_map>
#include"scanner.h"
#include<queue>
#include"parsing.hpp"
#include"ast.h"
using namespace std;

	int main(int argc, const char * argv[]) {
        unordered_map<string,int> x;
		// insert code here...
		//SCANNER s("/Users/naelkilani/Desktop/parser/parser/code2.txt");
		//queue<TOKEN *> * q;
		//q=s.Scan();
        Scope scope;
		SCANNER s("code.txt");
	/*SymbolTableEntry * t;
        SymbolTableEntry * entry=new SymbolTableEntry();
		entry->f.var.type=type_integer;
		entry->entry_type=ste_var;
		scope.current->table["sudqi"]=entry;

		entry=new SymbolTableEntry();
		entry->f.constant.type=type_integer;
		entry->f.constant.value=30;
		entry->entry_type=ste_const;

		cout<<scope.insert("dia",entry)<<endl;
		//cout<<scope.insert("dia",entry)<<endl;

		scope.enterScope();
		entry->f.constant.value=70;
		cout<<scope.insert("dia",entry)<<endl;
		entry->f.constant.type=type_string;
		entry->f.constant.str_value="hello";
		entry->entry_type=ste_const;

		scope.insert("nael",entry);
		//t=scope.current->table.find("nael");
		t=scope.getFirstOcc("dia");
        if(t->entry_type!=ste_undefined)
		cout<<t->f.constant.str_value<<endl;
        
		cout<<t->f.constant.value<<endl;*/
		Parser parser("test1.txt");
		ast_list * a = parser.parse_decl_list();
		FILE * fp = fopen("new.txt", "w");
		print_ast_list(fp, a, "", 0);

		std::cout << "Hello, World!\n";
		return 0;
	}
