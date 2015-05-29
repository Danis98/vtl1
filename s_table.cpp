#include <symbol_table.h>

void symbol_table::insert(NIdentifier id, enum entry_type type, enum entry_data_type data_type, bool init){
	symbol_table_entry entry;
	entry.identifier=id.name;
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

symbol_table_entry symbol_table::lookup(NIdentifier id, enum etry_type type, std::vector<enum entry_data_type> args){
	for(int i=0;i<entries.size();i++){
		if(entries[i].identifier==id.name){
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
		return parent->lookup(id);
	//If not found, display to the user his/her stupidity and exit
	std::cout<<"Undefined or incompatible symbol "<<id.name<<std::endl;
	exit(0);
}

//Generate symbol table
void NBlock::generate_symbol_table(symbol_table *table){
	for(NStatement *stmt : statements)
		stmt->generate_symbol_table(table);
}

void NExpressionStatement::generate_symbol_table(symbol_table *table){
	expression.generate_symbol_table(table);
}

void NMethodCall::generate_symbol_table(symbol_table *table){
	//Construct a vector of entry_data_type elements
	std::vector<enum entry_data_type> args;
	for(int i=0;i<arguments.size();i++){
		switch(arguments[i]->getTypeID()){
			case NODE_TYPE_INT:
				args.push_back(INT);
				break;
			case NODE_TYPE_DOUBLE:
				args.push_back(DOUBLE);
				break;
		}
	}
}

