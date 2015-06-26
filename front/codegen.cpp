#include <node.h>
#include "parser.hpp"

struct intermediate_form int_code;
int offset=0;

int occupied_temps=0, occupied_labels=0;

temp_var get_temp(){
	return "t"+(occupied_temps++);
}

label get_label(){
	return "L"+(occupied_labels++);
}

temp_var NInteger::codeGen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, ""+value, "", t);
	return t;
}

temp_var NDouble::codeGen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, ""+value, "", t);
	return t;
}

temp_var NIdentifier::codeGen(){
	temp_var p=lookup(name)->id;
	if(p==NULL){
		std::cout<<"[COMPILATION FAILED] Couldn't resolve name "<<name<<std::endl;
		exit(0);
	}
	return p;
}

temp_var NBoolean::codeGen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, ""+value, "", t);
	return t;
}

temp_var NString::codeGen(){
	temp_var t=get_temp();
	emit(OP_ASSIGN, ""+value, "", t);
	return t;
}

temp_var NMethodCall::codeGen(){
	for(NExpression* arg : arguments)
		emit(OP_PARAM, arg->codeGen(), "", "");
	temp_var t=get_temp();
	temp_var func_id=lookup(id.name)->id;
	emit(OP_CALL, func_id, arguments.size(), t);
	return t;
}

temp_var NBinaryOperator::codeGen(){
	temp_var t=get_temp();
	switch(op){
	//No break or returns, falls through until TCGE
	case TCEQ:
		emit(OP_CEQ, left.codeGen(), right.codeGen(), t);
		return t;
	case TCNE:
		emit(OP_CNE, left.codeGen(), right.codeGen(), t);
		return t;
	case TCLT:
		emit(OP_CLT, left.codeGen(), right.codeGen(), t);
		return t;
	case TCLE:
		emit(OP_CLE, left.codeGen(), right.codeGen(), t);
		return t;
	case TCGT:
		emit(OP_CGT, left.codeGen(), right.codeGen(), t);
		return t;
	case TCGE:
		emit(OP_CGE, left.codeGen(), right.codeGen(), t);
		return t;
	case TPLUS:
		emit(OP_ADD, left.codeGen(), right.codeGen(), t);
		return t;
	case TMINUS:
		emit(OP_SUB, left.codeGen(), right.codeGen(), t);
		return t;
	case TMUL:
		emit(OP_MUL, left.codeGen(), right.codeGen(), t);
		return t;
	case TMOD:
		emit(OP_MOD, left.codeGen(), right.codeGen(), t);
		return t;
	default:
		std::cout<<"[COMPILATION FAILED] Invalid operator "<<op<<std::endl;
		exit(0);
}

temp_var NAssignment::codeGen(){
	struct symbol_table_entry *e=lookup(left.name);
	temp_var p=e->id, t=get_temp();
	if(p==NULL){
		std::cout<<"[COMPILATION FAILED] Couldn't resolve name "<<name<<std::endl;
		exit(0);
	}
	setInitialized(left.name);
	expr_typecheck(&(this));
	emit(OP_ASSIGN, p, right.codegen(), t);
	return t;
}

temp_var NBlock::codeGen(){
	temp_var r=NULL;
	cur_table=*(cur_table.mktable());
	for(NStatement stmt : statements)
		if(stmt.getTypeID()==NODE_TYPE_RETURN)
			r=stmt.codegen();
		else
			stmt.codeGen();
	cur_table=*(cur_table.parent);
	return r;
}

temp_var NExpressionStatement::codeGen(){
	expression.codegen();
	return NULL;
}

temp_var NVariableDeclaration::codeGen(){
	if(hasExpr && get_data_type(type.name)!=expr_typecheck(assignmentExpr)){
		std::cout<<"[COMPILATION FAILED] Incompatible assignment\n";
		exit(0);
	}
	temp_var p=insert(id, VAR, get_type(type.name), hasExpr, offset)->id;
	emit(OP_ASSIGN, assignmentExpr.codegen(), "", p);
	return p;
}

temp_var NFunctionDeclaration::codeGen(){
	label func_label=insert(id.name, FUNC, get_type(type.name), true, 0)->id;
	enum data_type ret_type=expr_typecheck(&block);
	if(get_type(type.name)!=ret_type){
		std::cout<<"[COMPILATION FAILED] Function "<<id.name<<" returning a "
			<<data_type_names(ret_type)<<" value, "<<type.name<<" expected\n";
		exit(0);
	}
	cur_table=*(cur_table.mktable());

	for(NVariableDeclaration *arg : arguments)
		insert(arg->id.name, VAR, arg->type.name, true, offset);

	emit(OP_LABEL, func_label, "", "");
	block.codegen();

	cur_table=*(cur_table.parent);
}

temp_var NIfStatement::codeGen(){
	
}

temp_var NForStatement::codeGen(){}

temp_var NWhileStatement::codeGen(){}

temp_var NReturnStatement::codeGen(){
	tem_var t=expr.codegen();
	emit(OP_RET, t, "", "");
	return t;
}

temp_var NString::codeGen(){}

temp_var NString::codeGen(){}

temp_var NString::codeGen(){}
