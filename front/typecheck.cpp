#include <typecheck.h>

enum data_type expr_typecheck(NExpression *expr){
	enum data_type l, r;
	bool init=false;
	symbol_table_entry *e;
	switch(expr->getTypeID()){
		case NODE_TYPE_INT:
			return INT;
		case NODE_TYPE_DOUBLE:
			return DOUBLE;
		case NODE_TYPE_BOOLEAN:
			return BOOLEAN;
		case NODE_TYPE_STRING:
			return STRING;
		//Find the identifier in the symbol. We assume that this is a variable, since a 
		//methodcall would have been processed at this point (args should be empty)
		case NODE_TYPE_IDENT:
			e=lookup(((NIdentifier*)expr)->name, VAR);	
			if(e->initialized==false){
				std::cout<<"[COMPILATION FAILED] Uninitialized symbol "<<e->name<<"\n";
				exit(0);
			}
			return e->data_type;
		case NODE_TYPE_CALL:
			for(NExpression *e : ((NMethodCall*)expr)->arguments)
				expr_typecheck(e);
			return lookup(((NMethodCall*)expr)->id.name, FUNC)->data_type;
		case NODE_TYPE_BINOP:
			l=expr_typecheck(&((NBinaryOperator*)expr)->left);
			r=expr_typecheck(&((NBinaryOperator*)expr)->right);
			return eval_binop(l, r, ((NBinaryOperator*)expr)->op);
		case NODE_TYPE_ASSIGN:
			e=lookup(((NAssignment*)expr)->left.name, VAR);
			l=e->data_type;
			r=expr_typecheck(&((NAssignment*)expr)->right);
			if(l==r)
				return l;
			if(l==DOUBLE && r==INT)
				return DOUBLE;
			std::cout<<"[COMPILATION FAILED] Incompatible assignment.\n"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"}\n";
			exit(0);
		case NODE_TYPE_BLOCK:
			for(NStatement* stmt : ((NBlock*)expr)->statements)
				if(stmt->getTypeID()==NODE_TYPE_RETURN){
					if(!init){
						l=expr_typecheck(&(((NReturnStatement*)stmt)->returnExpr));
						init=true;
					}
					else{
						r=expr_typecheck(&(((NReturnStatement*)stmt)->returnExpr));
						if(l==r) l=r;
						else{
							std::cout<<"[COMPILATION FAILED] Incompatible returns\n";
							exit(0);
						}
					}
			}
			return init?l:VOID;
		default:
			std::cout<<"[COMPILATION FAILED] Weird expression.\n";
			exit(0);
	}
}

enum data_type eval_binop(enum data_type l, enum data_type r, int op){
	if(l==VOID||r==VOID){
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
		exit(0);
	}
	switch(op){
		//No break or returns, falls through until TCGE
		case TCEQ:
    		case TCNE:
    		case TCLT:
    		case TCLE:
    		case TCGT:
    		case TCGE:
			return BOOLEAN;
		case TPLUS:
			if(l==r)
				return l;
			if((l==INT && r==DOUBLE)||(r==INT && l==DOUBLE))
				return DOUBLE;
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
			exit(0);
		case TMINUS:
			if(l==r && l!=STRING)
				return l;
			if((l==INT && r==DOUBLE)||(r==INT && l==DOUBLE))
				return DOUBLE;
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
			exit(0);
		case TMUL:
			if(l==r && l!=STRING)
				return l;
			if((l==INT && r==DOUBLE)||(r==INT && l==DOUBLE))
				return DOUBLE;
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
			exit(0);
		case TDIV:
			if(l==r && l!=STRING)
				return l;
			if((l==INT && r==DOUBLE)||(r==INT && l==DOUBLE))
				return DOUBLE;
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
			exit(0);
		case TMOD:
			if(l==INT && r==INT)
				return INT;
			std::cout<<"[COMPILATION FAILED] Incompatible operands:"
			<<" {"<<data_type_names(l)<<","
			<<data_type_names(r)<<"} "<<" op: "<<getOp(op)<<"\n";
			exit(0);
		default:
			std::cout<<"[COMPILATION FAILED] Operator "<<op<<" undefined\n";
			exit(0);
	}
}

enum data_type get_data_type(std::string id){
	if(id=="int")
		return INT;
	else if(id=="double")
		return DOUBLE;
	else if(id=="bool")
		return BOOLEAN;
	else if(id=="string")
		return STRING;
	else if(id=="void")
		return VOID;
	else{
		std::cout<<"[COMPILATION FAILED] Unknown type "<<id<<std::endl;
		exit(0);
	}
}
