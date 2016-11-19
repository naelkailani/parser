//
//  main.cpp
//  parser
//
//  Created by Nael Kilani on 11/18/16.
//  Copyright © 2016 Nael Kilani. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include"scanner.h"
#include<queue>
using namespace std;
/* symbol.h */
//A symbol table entry holds a string and any other values that a programmer wants to add to it. */
typedef enum
{
	type_integer,
	type_string,
	type_boolean,
	//type_float,
	type_none
} j_type;
/* printable versions of their names. */
static char *type_names[] = {"integer", "string", "boolean", "float", "none"};
typedef enum
{
    	ste_var, // a variable
	ste_const, //A constant
	ste_routine, //A routine
	ste_undefined // ste_entry
} ste_entry_type;
// You may change the following definition to a class
class SymbolTableEntry
{
public:
	SymbolTableEntry(){
	}
	SymbolTableEntry(ste_entry_type type,j_type varType){
		if(type==ste_var){
			entry_type=type;
			this->f.var.type=varType;
		}
		else {
			printf("type allowd to create is var");
		}
	}
	SymbolTableEntry(ste_entry_type type,j_type structType,int value){
        entry_type=type;
		if(type==ste_const&&structType==type_integer){
			f.constant.type=structType;
			f.constant.value=value;
		}
		else if(type==ste_routine){
			f.routine.result_type=structType;
			f.routine.formalNumber=value;
		}
		else{
			//printf("just routine and integer constant type accept");
			throw new exception();
		}
	}
	SymbolTableEntry(ste_entry_type type,j_type constType,string str_value){
		if(type==ste_const&&constType==type_string){
			f.constant.type=constType;
			f.constant.str_value=str_value.data();
		}
		else 
            throw new exception();
	}

    string name;
	ste_entry_type entry_type;
	// User-modifiable fields go here:
	union{
		//l .for a variable record its type
		struct{
				j_type type;
			} var;
		// for a constant record its value
		struct{
			j_type type;
			int value;
            char const * str_value;
		} constant;
		/* for a routine, record formal parameters and result type */
		struct{
			// SteListCelll *formals;// will be defined later
			j_type result_type;
			int formalNumber;
		} routine;
	} f;
}; //end of SymbolTableEntry definition

class SymbolTable{
    
public:
    SymbolTable * prev;
    std::unordered_map <string , SymbolTableEntry *> table;
	SymbolTable(){
		prev=NULL;
	}
	SymbolTable(SymbolTable * s){
		prev=s;
	}

};

class Scope{
	SymbolTable * head;
	public :SymbolTable * current;
	Scope(){
		head=new SymbolTable();
		current=head;
	}

	void enterScope(){
        SymbolTable * newSymbolTable= new SymbolTable(current);
		current=newSymbolTable;

	}
	void exitScope(){
        if(current!=NULL)
		current=current->prev;

	}
    SymbolTableEntry * find(string name){
	std::unordered_map <string , SymbolTableEntry *>::iterator t =current->table.find(name);
	SymbolTableEntry * s=new SymbolTableEntry();
	if(t==current->table.end()){
		s->entry_type=ste_undefined;
		return s;
	}
	else 
		s=t->second;
	
	
        return s;
    }
    bool insert(string name,SymbolTableEntry * entry){
	    if(current->table.find(name)==current->table.end()){
		    current->table[name]=entry;
		    return true;
	    }
	    else{
		    return false;
	    }

    }
    SymbolTableEntry * getFirstOcc(string name){
	    SymbolTableEntry * entry=NULL;
	    SymbolTableEntry * tempEntry=NULL;
	    SymbolTable * currentOld=current;
	    while(current!=NULL){
		    tempEntry=find(name);
		    if(tempEntry->entry_type!=ste_undefined){
			    entry=tempEntry;
			    current=currentOld;
			    return entry;
		    }
		    exitScope();
	    }
	    current=currentOld;
        throw new exception;
	   // return tempEntry;
    }
	   

};

	int main(int argc, const char * argv[]) {
        unordered_map<string,int> x;
		// insert code here...
		//SCANNER s("/Users/naelkilani/Desktop/parser/parser/code2.txt");
		//queue<TOKEN *> * q;
		//q=s.Scan();
        Scope scope;
	SymbolTableEntry * t;
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
        
		cout<<t->f.constant.value<<endl;
        

		std::cout << "Hello, World!\n";
		return 0;
	}
