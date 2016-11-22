#include"symbol.h"
#include<iostream>
using namespace std;
SymbolTableEntry::SymbolTableEntry(){
}
SymbolTableEntry::SymbolTableEntry(string name,ste_entry_type type,j_type varType){
	if(type==ste_var){
		entry_type=type;
		this->f.var.type=varType;
	}
	else {
		cout<<("type allowd to create is var");
	}
}
SymbolTableEntry::SymbolTableEntry(string name,ste_entry_type type,j_type structType,int value){
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
SymbolTableEntry::SymbolTableEntry(string name,ste_entry_type type,j_type constType,string str_value){
	if(type==ste_const&&constType==type_string){
		f.constant.type=constType;
		f.constant.str_value=str_value.data();
	}
	else 
		throw new exception();
}


SymbolTable::SymbolTable(){
	prev=NULL;
}
SymbolTable::SymbolTable(SymbolTable * s){
	prev=s;
}

Scope::Scope(){
	head=new SymbolTable();
	current=head;
}

void Scope::enterScope(){
	SymbolTable * newSymbolTable= new SymbolTable(current);
	current=newSymbolTable;

}
void Scope::exitScope(){
	if(current!=NULL)
		current=current->prev;

}
SymbolTableEntry * Scope::find(string name){
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
bool Scope::insert(string name,SymbolTableEntry * entry){
	if(current->table.find(name)==current->table.end()){
		current->table[name]=entry;
		return true;
	}
	else{
		return false;
	}

}
SymbolTableEntry * Scope::getFirstOcc(string name){
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

