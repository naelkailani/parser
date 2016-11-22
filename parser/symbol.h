//
//  symbol.h
//  parser
//
//  Created by Nael Kilani on 11/19/16.
//  Copyright © 2016 Nael Kilani. All rights reserved.
//

#ifndef symbol_h
#define symbol_h
#include<string>
#include <unordered_map>
using namespace std;
typedef enum{
    type_integer,
    type_string,
    type_boolean,
    type_float,
    type_none
} j_type;
/* printable versions of their names. */
static char *type_names[] = {"integer", "string", "boolean", "float", "none"};
typedef enum {
    ste_var, // a variable
    ste_const, //A constant
    ste_routine, //A routine
    ste_undefined // ste_entry
} ste_entry_type;
// You may change the following definition to a class
class SymbolTableEntry{
public:
    SymbolTableEntry();
	SymbolTableEntry::SymbolTableEntry(string name, ste_entry_type type, int value);
    SymbolTableEntry::SymbolTableEntry(string name,ste_entry_type type,j_type varType);
    SymbolTableEntry(string name,ste_entry_type type,j_type structType,int value);
    
    
    
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
            int value;
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
    SymbolTable();
    
    SymbolTable(SymbolTable * s);
    
    
};
class Scope{
    SymbolTable * head;
    public :SymbolTable * current;
    Scope();
    
    
    void enterScope();
    void exitScope();
    SymbolTableEntry * find(string name);
    
    bool insert(string name,SymbolTableEntry * entry);
    SymbolTableEntry * getFirstOcc(string name);
    
	   
    
};


#endif /* symbol_h */
