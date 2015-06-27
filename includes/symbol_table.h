#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <data_types.h>
#include <int_codegen.h>

#define NULL_TABLE 0

struct symbol_table;

class NExpression;
enum data_type expr_typecheck(NExpression *expr, symbol_table *table);
enum data_type get_data_type(std::string id);

extern int ind;
extern struct symbol_table cur_table;

enum entry_type{
	FUNC,
	VAR
};

struct symbol_table_entry{
	//Original name
	std::string name;
	//Scope-specific name
	std::string id;
	enum data_type data_type;
	enum entry_type type;
	bool initialized;
	int width;
	int offset;
};

struct symbol_table{
	int subtable_size=0, size=0;
	std::string name;
	symbol_table *parent;
	std::vector<symbol_table_entry> entries;
	std::vector<symbol_table> local_tables;
	symbol_table(symbol_table *parent) : parent(parent){}
	struct symbol_table* mktable(){
		struct symbol_table child(this);
		child.name=name+"_"+std::to_string(subtable_size);
		local_tables.push_back(child);
		return &(local_tables[subtable_size++]);
	}
};

extern int offset;

void print_s_table(symbol_table *table);

struct symbol_table_entry *insert(std::string id,enum entry_type type, enum data_type d_type, bool init, int width, int offset);
struct symbol_table_entry *lookup(std::string id, enum entry_type type);
void set_initialized(std::string id);

#endif
