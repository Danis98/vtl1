#include <node.h>
#include "parser.hpp"
#include <typecheck.h>

enum data_type expected;
bool isFuncBlock=false;

std::string int_ops_dbg[]={
	"+",
	"-",
	"*",
	"/",
	"%",
	":=",
	"PARAM",
	"CALL",
	"JUMP",
	"JUMPIF",
	"JUMPNIF",
	"LABEL",
	"==",
	"!=",
	"<",
	"<=",
	">",
	">=",
	"RETURN"
};

std::string int_ops_str[]={
	"PLUS",
	"SUB",
	"MULT",
	"DIV",
	"MOD",
	"ASSIGN",
	"PARAM",
	"CALL",
	"JUMP",
	"JUMPIF",
	"JUMPNIF",
	"LABEL",
	"EQUAL",
	"NEQUAL",
	"LESS",
	"LESSEQ",
	"GREAT",
	"GREATEQ",
	"RETURN"
};

struct intermediate_form int_code;
int offset=0;

int occupied_temps=0, occupied_labels=0;

temp_var get_temp(){
	std::string n=to_string(occupied_temps++);
	temp_var t="t"+n;
	return t;
}

label get_label(){
	label L="L"+to_string(occupied_labels++);
	return L;
}

temp_var NInteger::codegen(){
	return to_string(value);
}

temp_var NDouble::codegen(){
	return to_string(value);
}

temp_var NIdentifier::codegen(){
	temp_var p=lookup(name, VAR)->id;	
	return p;
}

temp_var NBoolean::codegen(){
	return value?"true":"false";
}

temp_var NString::codegen(){
	return value;
}

temp_var NMethodCall::codegen(){
	for(int i=arguments.size()-1;i>=0;i--)
		emit(OP_PARAM, arguments[i]->codegen(), "", "");
	struct symbol_table_entry *e=lookup(id.name, FUNC);
	temp_var func_id=e->id;
	enum data_type dt=e->data_type;
	temp_var t=(dt==VOID)?"":get_temp();
	emit(OP_CALL, func_id, to_string(arguments.size()), t);
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
	case TDIV:
		emit(OP_DIV, left.codegen(), right.codegen(), t);
		return t;
	case TMOD:
		emit(OP_MOD, left.codegen(), right.codegen(), t);
		return t;
	default:
		fatal("[COMPILATION FAILED] Invalid operator "+get_op(op)+"\n");
	}
}

temp_var NAssignment::codegen(){
	struct symbol_table_entry *e=lookup(left.name, VAR);
	temp_var p=e->id;
	set_initialized(left.name);
	expr_typecheck(this);
	temp_var r=right.codegen();
	emit(OP_ASSIGN, r, "", p);
	
	return p;
}

temp_var NBlock::codegen(){
	temp_var r="";
	cur_table=mktable();
	//DON'T TOUCH! Magic stuff should not be messed with...
	insert("0", VAR, VOID, false, 0, 0);
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
	data_type expr_type;
	if(hasExpr)
		expr_type=expr_typecheck(assignmentExpr);
	if(hasExpr
		&& (get_data_type(type.name)!=expr_type
			|| (get_data_type(type.name)==DOUBLE && expr_type==INT)))
		fatal("[COMPILATION FAILED] Incompatible assignment\n");
	temp_var p=insert(id.name, VAR, get_data_type(type.name), hasExpr, get_width(get_data_type(type.name)), offset)->id;
	if(hasExpr)
		emit(OP_ASSIGN, hasExpr?assignmentExpr->codegen():"", "", p);
	return p;
}

temp_var NFunctionDeclaration::codegen(){
	//Jump past the function declaration when executing
	label next_main=get_label();
	emit(OP_JUMP, next_main, "", "");
	//Function label
	label func_label=insert(id.name, FUNC, get_data_type(type.name), true, 0, 0)->id;
	emit(OP_LABEL, func_label, "", "");
	
	//Enter local scope and generate code
	cur_table=mktable();
	//DON'T TOUCH! Magic stuff should not be messed with...
	insert("0", VAR, VOID, false, 0, 0);

	int arg_count=0;
	for(NVariableDeclaration *arg : arguments){
		temp_var p_id=insert(arg->id.name, VAR, get_data_type(arg->type.name), true, get_width(get_data_type(arg->type.name)), offset)->id;
		emit(OP_ASSIGN, "param_"+to_string(arg_count), "", p_id);
		arg_count++;
	}
	
	//Setup for typecheck
	isFuncBlock=true;
	expected=get_data_type(type.name);
	
	//Generate and check
	block.codegen();
	if(type.name=="void")
		emit(OP_RET, "", "", "");
	emit(OP_LABEL, next_main, "", "");
	
	isFuncBlock=false;

	cur_table=*(cur_table.parent);
	
	return "";
}

temp_var NIfStatement::codegen(){
	label false_label=get_label();
	label next=get_label();
	temp_var e=condition.codegen();
	emit(OP_JUMPNIF, e, false_label, "");
	ifBlock.codegen();
	emit(OP_JUMP, next, "", "");
	emit(OP_LABEL, false_label, "", "");
	if(hasElse)
		elseBlock->codegen();
	emit(OP_LABEL, next, "", "");
	return "";
}

temp_var NForStatement::codegen(){
	label begin=get_label();
	label next=get_label();
	initExpr->codegen();
	emit(OP_LABEL, begin, "", "");
	temp_var e=condition->codegen();
	emit(OP_JUMPNIF, e, next, "");
	forBlock.codegen();
	incrExpr->codegen();
	emit(OP_JUMP, begin, "", "");
	emit(OP_LABEL, next, "", "");
	return "";
}

temp_var NWhileStatement::codegen(){
	label begin=get_label();
	label next=get_label();
	emit(OP_LABEL, begin, "", "");
	temp_var e=condition.codegen();
	emit(OP_JUMPNIF, e, next, "");
	whileBlock.codegen();
	emit(OP_JUMP, begin, "", "");
	emit(OP_LABEL, next, "", "");
	return "";
}

temp_var NReturnStatement::codegen(){
	temp_var t=returnExpr.codegen();
	emit(OP_RET, t, "", "");
	return t;
}
