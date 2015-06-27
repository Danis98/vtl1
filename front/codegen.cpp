#include <node.h>
#include "parser.hpp"

std::string int_ops_str[]={
	"ADD",
	"SUB",
	"MULT",
	"DIV",
	"MOD",
	"ASSIGN",
	"PARAM",
	"CALL",
	"JUMP",
	"LABEL",
	"==",
	"!=",
	"<",
	"<=",
	">",
	">=",
	"RETURN"
};

struct intermediate_form int_code;
int offset=0;

int occupied_temps=0, occupied_labels=0;

temp_var get_temp(){
	return "t"+std::to_string(occupied_temps++);
}

label get_label(){
	return "L"+std::to_string(occupied_labels++);
}

temp_var NInteger::codegen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, std::to_string(value), "", t);
	return t;
}

temp_var NDouble::codegen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, std::to_string(value), "", t);
	return t;
}

temp_var NIdentifier::codegen(){
	return lookup(name, VAR)->id;
}

temp_var NBoolean::codegen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, value?"true":"false", "", t);
	return t;
}

temp_var NString::codegen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, value, "", t);
	return t;
}

temp_var NMethodCall::codegen(){
	for(NExpression* arg : arguments)
		emit(OP_PARAM, arg->codegen(), "", "");
	struct symbol_table_entry *e=lookup(id.name, FUNC);
	temp_var t=e->data_type==VOID?"":get_temp();
	temp_var func_id=e->id;
	emit(OP_CALL, func_id, std::to_string(arguments.size()), t);
	return t;
}

temp_var NBinaryOperator::codegen(){
	temp_var t=get_temp();
	switch(op){
	//No break or returns, falls through until TCGE
	case TCEQ:
		emit(OP_CEQ, left.codegen(), right.codegen(), t);
		return t;
	case TCNE:
		emit(OP_CNE, left.codegen(), right.codegen(), t);
		return t;
	case TCLT:
		emit(OP_CLT, left.codegen(), right.codegen(), t);
		return t;
	case TCLE:
		emit(OP_CLE, left.codegen(), right.codegen(), t);
		return t;
	case TCGT:
		emit(OP_CGT, left.codegen(), right.codegen(), t);
		return t;
	case TCGE:
		emit(OP_CGE, left.codegen(), right.codegen(), t);
		return t;
	case TPLUS:
		emit(OP_ADD, left.codegen(), right.codegen(), t);
		return t;
	case TMINUS:
		emit(OP_SUB, left.codegen(), right.codegen(), t);
		return t;
	case TMUL:
		emit(OP_MUL, left.codegen(), right.codegen(), t);
		return t;
	case TMOD:
		emit(OP_MOD, left.codegen(), right.codegen(), t);
		return t;
	default:
		std::cout<<"[COMPILATION FAILED] Invalid operator "<<op<<std::endl;
		exit(0);
	}
}

temp_var NAssignment::codegen(){
	struct symbol_table_entry *e=lookup(left.name, VAR);
	temp_var p=e->id, t=get_temp();
	set_initialized(left.name);
	expr_typecheck(this);
	emit(OP_ASSIGN, p, right.codegen(), t);
	return t;
}

temp_var NBlock::codegen(){
	temp_var r="";
	cur_table=*(cur_table.mktable());
	for(NStatement *stmt : statements)
		if(stmt->getTypeID()==NODE_TYPE_RETURN)
			r=stmt->codegen();
		else
			stmt->codegen();
	cur_table=*(cur_table.parent);
	return r;
}

temp_var NExpressionStatement::codegen(){
	expression.codegen();
	return "";
}

temp_var NVariableDeclaration::codegen(){
	if(hasExpr && get_data_type(type.name)!=expr_typecheck(assignmentExpr)){
		std::cout<<"[COMPILATION FAILED] Incompatible assignment\n";
		exit(0);
	}
	temp_var p=insert(id.name, VAR, get_data_type(type.name), hasExpr, get_width(get_data_type(type.name)), offset)->id;
	emit(OP_ASSIGN, assignmentExpr->codegen(), "", p);
	return p;
}

temp_var NFunctionDeclaration::codegen(){
	label func_label=insert(id.name, FUNC, get_data_type(type.name), true, 0, 0)->id;
	enum data_type ret_type=expr_typecheck(&block);
	if(get_data_type(type.name)!=ret_type){
		std::cout<<"[COMPILATION FAILED] Function "<<id.name<<" returning a "
			<<data_type_names(ret_type)<<" value, "<<type.name<<" expected\n";
		exit(0);
	}
	cur_table=*(cur_table.mktable());

	for(NVariableDeclaration *arg : arguments)
		insert(arg->id.name, VAR, get_data_type(arg->type.name), true, get_width(get_data_type(arg->type.name)), offset);

	emit(OP_LABEL, func_label, "", "");
	block.codegen();

	cur_table=*(cur_table.parent);
}

temp_var NIfStatement::codegen(){
	
}

temp_var NForStatement::codegen(){}

temp_var NWhileStatement::codegen(){}

temp_var NReturnStatement::codegen(){
	temp_var t=returnExpr.codegen();
	emit(OP_RET, t, "", "");
	return t;
}
