#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <data_types.h>

#define NULL_TABLE 0

struct symbol_table;

class NExpression;
enum data_type expr_typecheck(NExpression *expr);
enum data_type get_data_type(std::string id);

enum entry_type{
	FUNC,
	VAR
};

struct symbol_table_entry{
	std::string identifier;
	enum data_type data_type;
	enum entry_type type;
	bool initialized;
	//If it is a function, the types of the args
	std::vector<enum data_type> args;
	//Pointer to the sub-table, if existent
	symbol_table *local_table;
};

struct symbol_table{
	symbol_table *parent;
	std::vector<symbol_table_entry> entries;
	std::vector<symbol_table*> local_tables;
	symbol_table(symbol_table *parent) : parent(parent){}
	
	void insert(std::string id, enum entry_type type, enum data_type data_type, bool init);
	symbol_table_entry lookup(std::string id, enum entry_type type, std::vector<enum data_type> args);
};

#endif
