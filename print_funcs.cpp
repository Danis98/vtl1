#include "node.h"

using namespace std;

void do_indentation(int ind){
	cout<<ind<<"|";
	for(int i=0;i<ind;i++)
		cout<<"\t";
}

void Node::print(int ind){
	do_indentation(ind);
	cout<<"Generic node\n";
}

void NInteger::print(int ind){
	do_indentation(ind);
	cout<<"INTEGER:  "<<value<<endl;
}

void NDouble::print(int ind){
	do_indentation(ind);
	cout<<"DOUBLE:  "<<value<<endl;
}

void NIdentifier::print(int ind){
	do_indentation(ind);
	cout<<"IDENTIFIER:  "<<name<<endl;
}

void NMethodCall::print(int ind){
	do_indentation(ind);
	cout<<"METHOD CALL: "<<endl;
	id.print(ind+1);
	for(int i=0;i<arguments.size();i++)
		arguments[i]->print(ind+1);
}

void NBinaryOperator::print(int ind){
	do_indentation(ind);
	cout<<"BINARY OP: "<<op<<"\n";
	left.print(ind+1);
	right.print(ind+1);
}

void NAssignment::print(int ind){
	do_indentation(ind);
	cout<<"ASSIGNMENT: \n";
	left.print(ind+1);
	right.print(ind+1);
}

void NBlock::print(int ind){
	do_indentation(ind);
	cout<<"CODE BLOCK: "<<statements.size()<<" statements\n";
	for(int i=0;i<statements.size();i++){
		statements[i]->print(ind+1);
		if(i!=statements.size()-1) cout<<endl;
	}
}

void NExpressionStatement::print(int ind){
	do_indentation(ind);
	cout<<"EXPRESSION\n";
	expression.print(ind+1);
}

void NVariableDeclaration::print(int ind){
	do_indentation(ind);
	cout<<"VARIABLE DECLARATION\n";
	type.print(ind+1);
	id.print(ind+1);
	//If it has an expression assigned, print it
	if(assignmentExpr!=NULL)
		assignmentExpr->print(ind+1);
}

void NFunctionDeclaration::print(int ind){
	do_indentation(ind);
	cout<<"FUNCTION DECLARATION: "<<arguments.size()<<" arguments\n";
	type.print(ind+1);
	id.print(ind+1);
	for(int i=0;i<arguments.size();i++)
		arguments[i]->print(ind+1);
	block.print(ind+1);
}

void NIfStatement::print(int ind){
	do_indentation(ind);
	cout<<"IF-ELSE STATEMENT:\n";
	condition.print(ind+1);
	ifBlock.print(ind+1);
	if(hasElse){
		elseBlock->print(ind+1);
	}
}

void NWhileStatement::print(int ind){
	do_indentation(ind);
	cout<<"WHILE STATEMENT:\n";
	condition.print(ind+1);
	whileBlock.print(ind+1);
}
