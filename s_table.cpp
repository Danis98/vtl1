#include <symbol_table.h>
#include <node.h>

void symbol_table::insert(std::string id, enum entry_type type, enum data_type data_type, bool init){
	symbol_table_entry entry;
	entry.identifier=id;
	entry.type=type;
	entry.data_type=data_type;
	entry.initialized=init;
	entries.push_back(entry);
	if(type==FUNCTION){
		symbol_table loc_table(this);
		local_tables.push_back(&loc_table);
		entry.local_table=&loc_table;
	}
}

symbol_table_entry symbol_table::lookup(std::string id, enum entry_type type, std::vector<enum data_type> args){
	for(int i=0;i<entries.size();i++){
		if(entries[i].identifier==id){
			if(entries[i].type!=type||args.size()!=entries[i].args.size())
				continue;
			if(type==VAR)
				return entries[i];
			bool found=true;
			for(int j=0;j<entries[i].args.size();j++)
				if(entries[i].args[j]!=args[j]){
					found=false;
					break;
				}
			if(found) return entries[i];
		}
	}
	if(parent!=NULL_TABLE)
		return parent->lookup(id, type, args);
	//If not found, display to the user his/her stupidity and exit
	std::cout<<"Undefined or incompatible symbol "<<id<<std::endl;
	exit(0);
}

//Generate symbol table: these function should insert entries in the appropriate symbol table 
//(recursively) and check if there aren't any undefined symbols
void NInteger::generate_symbol_table(symbol_table *table){
	
}

void NDouble::generate_symbol_table(symbol_table *table){
	
}

void NIdentifier::generate_symbol_table(symbol_table *table){
	
}

void NString::generate_symbol_table(symbol_table *table){
	
}

void NMethodCall::generate_symbol_table(symbol_table *table){
	//Construct a vector of data_type elements
	std::vector<enum data_type> args;
	for(int i=0;i<arguments.size();i++){
		
	}
}

void NBinaryOperator::generate_symbol_table(symbol_table *table){
	
}

void NAssignment::generate_symbol_table(symbol_table *table){
	
}

void NBlock::generate_symbol_table(symbol_table *table){
	for(NStatement *stmt : statements)
		stmt->generate_symbol_table(table);
}

void NExpressionStatement::generate_symbol_table(symbol_table *table){
	expression.generate_symbol_table(table);
}

void NVariableDeclaration::generate_symbol_table(symbol_table *table){
	
}

void NFunctionDeclaration::generate_symbol_table(symbol_table *table){
	
}

void NIfStatement::generate_symbol_table(symbol_table *table){
	
}

void NForStatement::generate_symbol_table(symbol_table *table){
	
}

void NWhileStatement::generate_symbol_table(symbol_table *table){
	
}

void NReturnStatement::generate_symbol_table(symbol_table *table){
	
}
