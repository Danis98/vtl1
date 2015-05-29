#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <vector>
#include <string.h>

#include <typeid.h>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node{
public:
	virtual ~Node(){}
	virtual void print(int ind)=0;
	virtual int getTypeID()=0;
	virtual void generate_symbol_table(symbol_table *table)=0;
};

class NExpression : public Node{
public:
	virtual void print(int ind)=0;
	virtual int getTypeID()=0;
	virtual void generate_symbol_table(symbol_table *table)=0;
};

class NStatement : public Node{
public:
	virtual void print(int ind)=0;
	virtual int getTypeID()=0;
	virtual void generate_symbol_table(symbol_table *table)=0;
};

class NInteger : public NExpression{
public:
	long long value;
	NInteger(long long value) : value(value){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_INT;}
	void generate_symbol_table(symbol_table *table);
};

class NDouble : public NExpression{
public:
	double value;
	NDouble(double value) : value(value){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_DOUBLE;}
	void generate_symbol_table(symbol_table *table);
};

class NIdentifier : public NExpression{
public:
	std::string name;
	NIdentifier(std::string& name) : name(name){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_IDENT;}
	void generate_symbol_table(symbol_table *table);
};

class NString : public NExpression{
public:
	std::string value;
	NString(std::string& val) : value(val){
		value=val.substr(1, val.length()-2);
	}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_STRING;}
	void generate_symbol_table(symbol_table *table);
};

class NMethodCall : public NExpression{
public:
	NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(NIdentifier& id, ExpressionList& arguments) : id(id), arguments(arguments){}
	NMethodCall(NIdentifier& id): id(id){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_CALL;}
	void generate_symbol_table(symbol_table *table);
};

class NBinaryOperator : public NExpression{
public:
	int op;
	NExpression& left;
	NExpression& right;
	NBinaryOperator(NExpression& left, int op, NExpression& right):
			left(left), right(right), op(op){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_BINOP;}
	void generate_symbol_table(symbol_table *table);
};

class NAssignment : public NExpression{
public:
	NIdentifier& left;
	NExpression& right;
	NAssignment(NIdentifier& left, NExpression& right): left(left), right(right){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_ASSIGN;}
	void generate_symbol_table(symbol_table *table);
};

class NBlock : public NExpression {
public:
	StatementList statements;
	NBlock(){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_BLOCK;}
	void generate_symbol_table(symbol_table *table);
};

class NExpressionStatement : public NStatement{
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression): expression(expression){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_EXPR_STMT;}
	void generate_symbol_table(symbol_table *table);
};

class NVariableDeclaration : public NStatement {
public:
	NIdentifier& type;
	NIdentifier& id;
	NExpression *assignmentExpr;
	bool hasExpr=false;
	NVariableDeclaration(NIdentifier& type, NIdentifier& id) :
        type(type), id(id) { }
	NVariableDeclaration(NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
				type(type), id(id), assignmentExpr(assignmentExpr) {hasExpr=true;}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_VAR_DECL;}
	void generate_symbol_table(symbol_table *table);
};

class NFunctionDeclaration : public NStatement {
public:
	NIdentifier& type;
	NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(NIdentifier& type, NIdentifier& id, 
				VariableList& arguments, NBlock& block) :
        type(type), id(id), arguments(arguments), block(block) { }
	void print(int ind);
	int getTypeID(){return NODE_TYPE_FUNC;}
	void generate_symbol_table(symbol_table *table);
};

class NIfStatement : public NStatement{
public:
	NExpression& condition;
	NBlock& ifBlock;
	NBlock *elseBlock;
	bool hasElse=false;
	NIfStatement (NExpression& condition, NBlock& ifBlock) : 
		condition(condition), ifBlock(ifBlock){}
	NIfStatement (NExpression& condition, NBlock& ifBlock, NBlock *elseBlock) : 
		condition(condition), ifBlock(ifBlock), elseBlock(elseBlock){hasElse=true;}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_IF;}
	void generate_symbol_table(symbol_table *table);
};

class NWhileStatement : public NStatement{
public:
	NExpression& condition;
	NBlock& whileBlock;
	NWhileStatement (NExpression& condition, NBlock& whileBlock) : 
		condition(condition), whileBlock(whileBlock){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_WHILE;}
	void generate_symbol_table(symbol_table *table);
};

class NForStatement : public NStatement{
public:
	NExpression *initExpr;
	NExpression *condition;
	NExpression *incrExpr;
	NBlock& forBlock;
	NForStatement (NExpression *initExpr, NExpression *condition,
			NExpression *incrExpr, NBlock& forBlock) : 
		initExpr(initExpr), condition(condition),
		incrExpr(incrExpr), forBlock(forBlock){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_FOR;}
	void generate_symbol_table(symbol_table *table);
};

class NReturnStatement : public NStatement{
public:
	NExpression& returnExpr;
	NReturnStatement (NExpression& returnExpr) : returnExpr(returnExpr){}
	void print(int ind);
	int getTypeID(){return NODE_TYPE_RETURN;}
	void generate_symbol_table(symbol_table *table);
};

#endif
