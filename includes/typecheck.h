#ifndef _TYPECHECK_H
#define _TYPECHECK_H

#include <node.h>

extern symbol_table s_table;

enum data_type expr_typecheck(NExpression *expr, symbol_table *table);
enum data_type eval_binop(enum data_type l, enum data_type r, int op);
enum data_type get_data_type(std::string id);

#endif
