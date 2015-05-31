#include <typecheck.h>

enum data_type expr_typecheck(NExpression *expr){
	switch(expr->getTypeID()){
		case NODE_TYPE_INT:
			return INT;
		case NODE_TYPE_DOUBLE:
			return DOUBLE;
	}
}
