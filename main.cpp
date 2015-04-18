#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include "node.h"

using namespace std;

//Root
extern NBlock* programBlock;
extern int yyparse();

extern FILE *yyin;

inline bool file_exists(const char *name){
	struct stat buffer;
	return stat(name, &buffer)==0;
}

int main(int argc, char **argv){
	if(argc<2){
		cout<<"Usage: "<<argv[0]<<"[ -f ] <vtl source file>, "<<argc-1<<" arguments inputted instead\n";
		return 0;
	}
	
	for(int i=1;i<argc;i++){
		if(file_exists(argv[i]))
			yyin=fopen(argv[i],"r");
		else{
			cout<<"Invalid argument/s"<<endl;
			return 0;
		}
	}
	
	//Parse
	cout<<"Parsing file "<<argv[1]<<"...\n";
	yyparse();

	//Print the resulting AST
	programBlock->print(0);
	return 0;
}
