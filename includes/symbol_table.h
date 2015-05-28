#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <node.h>

#define NULL_TABLE 0

struct symbol_table;

enum entry_data_type{
	VOID,
	INT,
	DOUBLE,
	STRING
};

enum entry_type{
	FUNCTION,
	VAR
};

struct symbol_table_entry{
	std::string identifier;
	enum entry_data_type data_type;
	enum entry_type type;
	bool initialized;
	//Pointer to the sub-table, if existent
	symbol_table *local_table;
};

struct symbol_table{
	symbol_table *parent;
	std::vector<symbol_table_entry> entries;
	std::vector<symbol_table*> local_tables;
	symbol_table(symbol_table *parent) : parent(parent){}
	
	void insert(NIdentifier id, enum entry_type type, enum entry_data_type data_type, bool init);
	symbol_table_entry lookup(NIdentifier id);
};

#endif
