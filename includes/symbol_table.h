#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <vector>

enum entry_type{
	VAR,
	FUNC
};

struct symbol_table_entry{
	char *identifier;
	enum entry_type type;
	//Pointer to local symbol table if the symbol is a function
	symbol_table *local_table;
};

struct symbol_table{
	vector<symbol_table_entry> entries;
	//Subordinate tables (if-else statements, loops etc...)
	vector<symbol_table> local_tables;
};

#endif
