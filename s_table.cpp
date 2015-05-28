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

symbol_table_entry symbol_table::lookup(NIdentifier id){
	for(int i=0;i<entries.size();i++)
		if(entries[i].identifier==id.name)
			return entries[i];
	if(parent!=NULL_TABLE)
		return parent->lookup(id);
	//If not found, display to the user his/her stupidity and exit
	std::cout<<"Undefined symbol "<<id.name<<std::endl;
	exit(0);
}
