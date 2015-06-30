#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <string>
#include <data_types.h>
#include <int_codegen.h>

#define NULL_TABLE 0

struct symbol_table;

class NExpression;
enum data_type expr_typecheck(NExpression *expr);
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
	symbol_table(symbol_table *parent) : parent(parent){
		name=parent->name+"_"+to_string(parent->subtable_size++);	
	}
	symbol_table(){parent==NULL_TABLE; name="";}
};

extern int offset;

struct symbol_table_entry *insert(std::string id,enum entry_type type, enum data_type d_type, bool init, int width, int offset);
struct symbol_table_entry *lookup(std::string id, enum entry_type type);
void set_initialized(std::string id);
inline symbol_table mktable(){return new symbol_table(cur_table);}

extern std::string sys_func_names[];
extern symbol_table_entry sys_funcs[];
extern int sys_funcs_num;

int check_sys(std::string id);
symbol_table_entry* get_sys_func_data(int id);

#endif
