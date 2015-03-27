#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

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
	//virtual llvm::Value* codeGen(CodeGenContext& context){}
};

class NExpression : public Node{};

class NStatement : public Node{};

class NInteger : public NExpression{
public:
	long long value;
	NInteger(long long value) : value(value){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression{
public:
	double value;
	NDouble(double value) : value(value){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIdentifier : public NExpression{
public:
	std::string name;
	NIdentifier(const std::string& name) : name(name){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression{
public:
	const NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(const NIdentifier& id, ExpressionList& arguments) : id(id), arguments(arguments){}
	NMethodCall(const NIdentifier& id): id(id){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression{
public:
	int op;
	NExpression& left;
	NExpression& right;
	NBinaryOperator(NExpression& left, int op, NExpression& right):
			left(left), right(right), op(op){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression{
public:
	NIdentifier& left;
	NExpression& right;
	NAssignment(NIdentifier& left, NExpression& right): left(left), right(right){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
	StatementList statements;
	NBlock(){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement{
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression): expression(expression){}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    NIdentifier& id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
        type(type), id(id) { }
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
        type(type), id(id), assignmentExpr(assignmentExpr) { }
    //virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, 
            const VariableList& arguments, NBlock& block) :
        type(type), id(id), arguments(arguments), block(block) { }
    //virtual llvm::Value* codeGen(CodeGenContext& context);
};
