#include <symbol_table.h>
#include <node.h>

struct symbol_table_entry *insert(std::string name, enum entry_type type, enum data_type data_type, bool init, int width, int offset){
	symbol_table_entry entry;
	entry.name=name;
	entry.id=cur_table.name+(type==FUNC?"_f_":"_")+name;
	entry.type=type;
	entry.data_type=data_type;
	entry.initialized=init;
	cur_table.entries.push_back(entry);
	return &(cur_table.entries[cur_table.size++]);
}

void set_initialized(std::string id){
	for(int i=0;i<cur_table.size;i++){
		if(cur_table.entries[i].name==id && cur_table.entries[i].type==VAR){
			cur_table.entries[i].initialized=true;
			return;
		}
	}
	symbol_table *table_ptr=cur_table.parent;
	
	while(table_ptr!=NULL_TABLE)
		for(int i=0;i<table_ptr->size;i++)
			if(table_ptr->entries[i].name==id && table_ptr->entries[i].type==VAR){
				table_ptr->entries[i].initialized=true;
				return;
			}
		
	std::cout<<"[COMPILATION FAILED] Undefined symbol "<<id<<"\n";
	exit(0);
}

symbol_table_entry *lookup(std::string id, enum entry_type type){
	for(int i=0;i<cur_table.size;i++)
		if(cur_table.entries[i].name==id && cur_table.entries[i].type==type)
			return &(cur_table.entries[i]);
	
	symbol_table table=cur_table;

	do{
		table=*(table.parent);
		if(table.name==table.parent->name) exit(0);
		for(int i=0;i<table.size;i++){
			if(table.entries[i].name==id && table.entries[i].type==type)
				return &(table.entries[i]);
		}
	}
	while(table.parent!=NULL_TABLE);
	
	std::cout<<"[COMPILATION FAILED] Undefined symbol "<<id<<"\n";
	exit(0);
}
