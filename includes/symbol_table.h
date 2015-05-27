#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <vector>

#define NULL_TABLE 0

enum entry_type{
	VOID,
	INT,
	DOUBLE,
	STRING
};

struct symbol_table_entry{
	char *identifier;
	enum entry_type type;
	symbol_table *local_table;
};

struct symbol_table{
	symbol_table *parent;
	vector<symbol_table_entry> entries;
	symbol_table(symbol_table *parent) : parent(parent){}
};

#endif
