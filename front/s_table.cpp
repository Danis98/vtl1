#include <symbol_table.h>
#include <node.h>

void symbol_table::insert(std::string id, enum entry_type type, enum data_type data_type, bool init){
	symbol_table_entry entry;
	entry.identifier=id;
	entry.type=type;
	entry.data_type=data_type;
	entry.initialized=init;
	entries.push_back(entry);
	if(type==FUNC){
		symbol_table loc_table(this);
		local_tables.push_back(&loc_table);
		entry.local_table=&loc_table;
	}
}

symbol_table_entry symbol_table::lookup(std::string id, enum entry_type type, std::vector<enum data_type> args){
	for(int i=0;i<entries.size();i++){
		if(entries[i].identifier==id){
			if(entries[i].type!=type||args.size()!=entries[i].args.size())
				continue;
			if(type==VAR)
				return entries[i];
			bool found=true;
			for(int j=0;j<entries[i].args.size();j++)
				if(entries[i].args[j]!=args[j]){
					found=false;
					break;
				}
			if(found) return entries[i];
		}
	}
	if(parent!=NULL_TABLE)
		return parent->lookup(id, type, args);
	//If not found, display to the user his/her stupidity and exit
	std::cout<<"[COMPILATION FAILED]Undefined or incompatible symbol "<<id<<std::endl;
	exit(0);
}

//Generate symbol table: these function should insert entries in the appropriate symbol table 
//(recursively) and check if there aren't any undefined symbols

//Primitives don't need these things
void NInteger::generate_symbol_table(symbol_table *table){}

void NDouble::generate_symbol_table(symbol_table *table){}

void NString::generate_symbol_table(symbol_table *table){}

void NIdentifier::generate_symbol_table(symbol_table *table){
	std::vector<enum data_type> args;
	table->lookup(name, VAR, args);
}

void NMethodCall::generate_symbol_table(symbol_table *table){
	//Construct a vector of data_type elements
	std::vector<enum data_type> args;
	for(int i=0;i<arguments.size();i++){
		args.push_back(expr_typecheck(arguments[i]));
	}
	//Check if the method itself exists
	table->lookup(id.name, FUNC, args);
	//Do symbol table operations on args. Eventual symbol declarations inside the args
	//are local
	symbol_table loc_table(table);
	table->local_tables.push_back(&loc_table);
	for(int i=0;i<arguments.size();i++)
		arguments[i]->generate_symbol_table(&loc_table);
}

void NBinaryOperator::generate_symbol_table(symbol_table *table){
	left.generate_symbol_table(table);
	left.generate_symbol_table(table);
}

void NAssignment::generate_symbol_table(symbol_table *table){
	//Check if the variable exists, then do the same on the symbols of the expression
	//Finally, check if the types are compatible
	std::vector<enum data_type> args;
	enum data_type l_type=table->lookup(left.name, VAR, args).data_type;
	right.generate_symbol_table(table);
	enum data_type r_type=expr_typecheck(&right);
	if(l_type!=r_type && !(l_type==DOUBLE && r_type==INT)){
		std::cout<<"[COMPILATION FAILED]Incompatible assignment operands.\n";
		exit(0);
	}
}

void NBlock::generate_symbol_table(symbol_table *table){
	for(NStatement *stmt : statements)
		stmt->generate_symbol_table(table);
}

void NExpressionStatement::generate_symbol_table(symbol_table *table){
	expression.generate_symbol_table(table);
	expr_typecheck(&expression);
}

void NVariableDeclaration::generate_symbol_table(symbol_table *table){
	//Search in current scope for a variable also named like that
	std::vector<enum data_type> args;
	for(int i=0;i<table->entries.size();i++)
		if(table->entries[i].identifier==id.name){
			std::cout<<"[COMPILATION FAILED]Symbol "<<id.name
				<<" already defined in this scope\n";
			exit(0);
		}
	//Perform typecheck and insert
	enum data_type t=get_data_type(type.name);
	if(hasExpr){
		enum data_type ex_t=expr_typecheck(assignmentExpr);
		if(t!=ex_t && !(t==DOUBLE && ex_t==INT)){
			std::cout<<"[COMPILATION FAILED]Incompatible assignment operands"
				<<"at declaration of "<<id.name<<".\n";
			exit(0);
		}
	}
	if(t==VOID){
		std::cout<<"[COMPILATION FAILED]A variable cannot be of void type\n";
		exit(0);
	}
	table->insert(id.name,
			VAR,
			t,
			hasExpr);
}

void NFunctionDeclaration::generate_symbol_table(symbol_table *table){
	
}

void NIfStatement::generate_symbol_table(symbol_table *table){
	
}

void NForStatement::generate_symbol_table(symbol_table *table){
	
}

void NWhileStatement::generate_symbol_table(symbol_table *table){
	
}

void NReturnStatement::generate_symbol_table(symbol_table *table){
	
}
