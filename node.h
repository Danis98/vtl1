#include <iostream>
#include <vector>
#include <cstdio>

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
	virtual void print()=0;
	//virtual llvm::Value* codeGen(CodeGenContext& context){}
};

class NExpression : public Node{
public:
	virtual void print()=0;
};

class NStatement : public Node{
public:
	virtual void print()=0;
};

class NInteger : public NExpression{
public:
	long long value;
	NInteger(long long value) : value(value){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression{
public:
	double value;
	NDouble(double value) : value(value){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIdentifier : public NExpression{
public:
	std::string name;
	NIdentifier(std::string& name) : name(name){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression{
public:
	NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(NIdentifier& id, ExpressionList& arguments) : id(id), arguments(arguments){}
	NMethodCall(NIdentifier& id): id(id){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression{
public:
	int op;
	NExpression& left;
	NExpression& right;
	NBinaryOperator(NExpression& left, int op, NExpression& right):
			left(left), right(right), op(op){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression{
public:
	NIdentifier& left;
	NExpression& right;
	NAssignment(NIdentifier& left, NExpression& right): left(left), right(right){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
	StatementList statements;
	NBlock(){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement{
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression): expression(expression){}
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
	NIdentifier& type;
	NIdentifier& id;
	NExpression *assignmentExpr;
	NVariableDeclaration(NIdentifier& type, NIdentifier& id) :
        type(type), id(id) { }
	NVariableDeclaration(NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
				type(type), id(id), assignmentExpr(assignmentExpr) { }
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
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
	void print();
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};
