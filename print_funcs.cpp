#include "node.h"

using namespace std;

void NInteger::print(){
	cout<<"INTEGER:  "<<value<<endl;
}

void NDouble::print(){
	cout<<"DOUBLE:  "<<value<<endl;
}

void NIdentifier::print(){
	cout<<"IDENTIFIER:  "<<name<<endl;
}

void NMethodCall::print(){
	cout<<"METHOD CALL: "<<endl;
	cout<<"\t";
	id.print();
	for(int i=0;i<arguments.size();i++){
		cout<<"\t";
		arguments[i]->print();
	}
}

void NBinaryOperator::print(){
	cout<<"BINARY OP: "<<op<<"\n\t";
	left.print();
	cout<<"\t";
	right.print();
}

void NAssignment::print(){
	cout<<"ASSIGNMENT: "<<"\n\t";
	left.print();
	cout<<"\t";
	right.print();
}

void NBlock::print(){
	cout<<"CODE BLOCK: "<<endl;
	for(int i=0;i<statements.size();i++){
		cout<<"\t";
		statements[i]->print();
	}
}

void NExpressionStatement::print(){
	expression.print();
}

void NVariableDeclaration::print(){
	cout<<"VARIABLE DECLARTION\n";
	cout<<"\t";
	type.print();
	cout<<"\t";
	id.print();
	cout<<"\t";
	assignmentExpr->print();
}

void NFunctionDeclaration::print(){
	cout<<"FUNCTION DECLARTION\n";
	cout<<"\t";
	type.print();
	cout<<"\t";
	id.print();
	cout<<"\t";
	for(int i=0;i<arguments.size();i++){
		cout<<"\t";
		arguments[i]->print();
	}
	cout<<"\t";
	block.print();
}
