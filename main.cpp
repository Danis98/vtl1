#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>

#include <node.h>
#include <symbol_table.h>
#include <typecheck.h>

using namespace std;

//Root AST
extern NBlock* programBlock;
//Symbol table
struct symbol_table cur_table;
int ind=0;
//Output file
std::ofstream outfile;

extern int yyparse();

extern FILE *yyin;

inline bool file_exists(const char *name){
	struct stat buffer;
	return stat(name, &buffer)==0;
}

int main(int argc, char **argv){
	if(argc!=2){
		cout<<"Usage: "<<argv[0]<<"[ -f ] <vtl source file>, "
		<<argc-1<<" arguments inputted instead\n";
		return 0;
	}
	
	std::string filename(argv[1]);
	
	filename=filename.substr(0, filename.find_last_of("."));
	
	outfile.open(filename+".vvm");

	if(file_exists(argv[1]))
		yyin=fopen(argv[1],"r");
	else{
		cout<<"Invalid argument/s"<<endl;
		return 0;
	}
	
	//Parse
	//cout<<"Parsing file "<<argv[1]<<"...\n";
	yyparse();

	//Print the resulting AST
	//programBlock->print(0);

	//Generate intermediate code
	programBlock->codegen();
	
	return 0;
}
