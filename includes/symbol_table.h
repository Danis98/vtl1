#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <vector>

#define NULL_PARENT 0

enum entry_type{
	VOID,
	INT,
	DOUBLE,
	STRING
};

struct symbol_table_entry{
	char *identifier;
	enum entry_type type;
};

struct symbol_table{
	symbol_table *parent;
	vector<symbol_table_entry> entries;
	//Subordinate tables (if-else statements, loops etc...)
	vector<symbol_table*> local_tables;
	symbol_table(symbol_table *parent) : parent(parent){}
};

#endif
