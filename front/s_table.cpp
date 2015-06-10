#include <symbol_table.h>
#include <node.h>

void print_s_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"s_table: "<<table->size<<" elements\n";
	ind++;
	for(int i=0;i<table->size;i++){
		for(int j=0;j<ind;j++) std::cout<<"\t";
		std::cout<<"\t"<<table->entries[i].identifier<<
			"\t"<<(table->entries[i].type==FUNC?"FUNC":"VAR")<<
			"\t"<<data_type_names(table->entries[i].data_type)<<
			"\targs: "<<table->entries[i].args.size()<<
			"\tinit: "<<(table->entries[i].initialized?"true":"false")<<std::endl;
	}
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"Local tables: "<<table->local_tables.size()<<" subtables\n";
	ind++;
	for(int i=0;i<table->local_tables.size();i++)
		print_s_table(&(table->local_tables[i]));
	ind--;
}

void symbol_table::insert(std::string id, enum entry_type type, enum data_type data_type, std::vector<enum data_type> args, bool init){
	symbol_table_entry entry;
	entry.identifier=id;
	entry.type=type;
	entry.data_type=data_type;
	entry.initialized=init;
	if(type==FUNC)
		entry.args=args;
	entries.push_back(entry);
	size++;
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
	if(parent!=NULL_TABLE){
		return parent->lookup(id, type, args);
	}
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
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Identifier: "<<name<<std::endl;
	ind++;
	std::vector<enum data_type> args;
	table->lookup(name, VAR, args);
	ind--;
}

void NMethodCall::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Method call: "<<id.name<<std::endl;
	ind++;
	//Construct a vector of data_type elements
	std::vector<enum data_type> args;
	for(int i=0;i<arguments.size();i++){
		args.push_back(expr_typecheck(arguments[i], table));
	}
	//Check if the method itself exists
	table->lookup(id.name, FUNC, args);
	//Do symbol table operations on args. Eventual symbol declarations inside the args
	//are local
	symbol_table loc_table(table);
	for(int i=0;i<arguments.size();i++)
		arguments[i]->generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	ind--;
}

void NBinaryOperator::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Bin-op: "<<op<<std::endl;
	ind++;
	left.generate_symbol_table(table);
	right.generate_symbol_table(table);
	ind--;
}

void NAssignment::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Assignment: "<<left.name<<std::endl;
	ind++;
	//Check if the variable exists, then do the same on the symbols of the expression
	//Finally, check if the types are compatible
	std::vector<enum data_type> args;
	enum data_type l_type=table->lookup(left.name, VAR, args).data_type;
	right.generate_symbol_table(table);
	enum data_type r_type=expr_typecheck(&right, table);
	if(l_type!=r_type && !(l_type==DOUBLE && r_type==INT)){
			std::cout<<"[COMPILATION FAILED]Incompatible assignment operands:"
			<<" {"<<data_type_names(l_type)<<","
			<<data_type_names(r_type)<<"}\n";
		exit(0);
	}
	ind--;
}

void NBlock::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Block: "<<std::endl;
	ind++;
	symbol_table loc_table(table);
	for(NStatement *stmt : statements)
		stmt->generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	ind--;
}

void NExpressionStatement::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Expression statement: "<<std::endl;
	ind++;
	expression.generate_symbol_table(table);
	expr_typecheck(&expression, table);
	ind--;
}

void NVariableDeclaration::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Variable declaration: "<<id.name<<" {"<<type.name<<"}"<<std::endl;
	ind++;
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
		enum data_type ex_t=expr_typecheck(assignmentExpr, table);
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
			args,
			hasExpr);
	ind--;
}

void NFunctionDeclaration::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Function declaration: "<<id.name<<" {"<<type.name<<"}"<<std::endl;
	ind++;
	//Make sure the symbol doesn't exist, the make a local symbol table and fill it
	std::vector<enum data_type> args;
	for(int i=0;i<arguments.size();i++)
		args.push_back(get_data_type(arguments[i]->type.name));

	for(int i=0;i<table->entries.size();i++)
		if(table->entries[i].identifier==id.name){
			std::cout<<"[COMPILATION FAILED]Symbol "<<id.name
				<<" already defined in this scope\n";
			exit(0);
		}
	enum data_type t=get_data_type(type.name);
	table->insert(id.name,
			FUNC,
			t,
			args,
			false);
	symbol_table loc_table(table);
	for(int i=0;i<arguments.size();i++)
		arguments[i]->generate_symbol_table(&loc_table);
	block.generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	ind--;
}

void NIfStatement::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] If statement: "<<std::endl;
	ind++;
	symbol_table loc_table(table);
	condition.generate_symbol_table(&loc_table);
	ifBlock.generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	//Symbols declared in if condition should not be visible
	elseBlock->generate_symbol_table(table);
	ind--;
}

void NForStatement::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] For statement: "<<std::endl;
	ind++;
	symbol_table loc_table(table);
	initExpr->generate_symbol_table(&loc_table);
	condition->generate_symbol_table(&loc_table);
	incrExpr->generate_symbol_table(&loc_table);
	forBlock.generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	ind--;
}

void NWhileStatement::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] While statement: "<<std::endl;
	ind++;
	symbol_table loc_table(table);
	condition.generate_symbol_table(&loc_table);
	whileBlock.generate_symbol_table(&loc_table);
	table->local_tables.push_back(loc_table);
	ind--;
}

void NReturnStatement::generate_symbol_table(symbol_table *table){
	for(int i=0;i<ind;i++) std::cout<<"\t";
	std::cout<<"[S_TABLE] Return statement: "<<std::endl;
	ind++;
	returnExpr.generate_symbol_table(table);
	ind--;
}
