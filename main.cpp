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
//Debug option for more human-readable code
bool debug;

extern int yyparse();

extern FILE *yyin;

inline bool file_exists(const char *name){
	struct stat buffer;
	return stat(name, &buffer)==0;
}

int main(int argc, char **argv){
	if(argc<2 || argc>3){
		cout<<"Usage: "<<argv[0]<<" <vtl source file> [-debug], "
		<<argc-1<<" arguments inputted instead\n";
		for(int i=0;i<argc;i++)
			cout<<argv[i]<<"\n";
		return 0;
	}
	
	bool inputFileSubmitted=false;
	std::string filename;
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i], "-debug")==0){
			debug=true;
		}
		else{
			if(inputFileSubmitted)
				cout<<"[COMPILATION FAILED] Invalid arguments\n";
			filename=argv[i];
			inputFileSubmitted=true;
		}
	}
	
	if(file_exists(filename.c_str()))
		yyin=fopen(filename.c_str(),"r");
	else{
		cout<<"[COMPILATION FALED] Source file not found\n";
		return 0;
	}
	
	filename=filename.substr(0, filename.find_last_of("."));
	
	outfile.open(filename+".vvm"+(debug?"-dbg":""));

	//Parse
	//cout<<"Parsing file "<<argv[1]<<"...\n";
	yyparse();

	//Print the resulting AST
	//programBlock->print(0);

	//Generate intermediate code
	programBlock->codegen();
	
	return 0;
}
