#ifndef _TYPECHECK_H
#define _TYPECHECK_H

#include <node.h>
#include "../front/parser.hpp"

extern symbol_table s_table;

enum data_type expr_typecheck(NExpression *expr, symbol_table *table);
enum data_type eval_binop(enum data_type l, enum data_type r, int op);
enum data_type get_data_type(std::string id);

inline std::string getOp(int op){
	switch(op){
		case TCEQ:
			return "=";
		case TCNE:
			return "!=";
		case TCLT:
			return "<";
		case TCLE:
			return "<=";
		case TCGT:
			return ">";
		case TCGE:
			return ">=";
		case TPLUS:
			return "+";
		case TMINUS:
			return "-";
		case TMUL:
			return "*";
		case TDIV:
			return "/";
		case TMOD:
			return "%";
		default:
			return "UNKNOWN OPERATOR";
	}
}

#endif
