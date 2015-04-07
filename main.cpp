#include <iostream>
#include "node.h"

using namespace std;

extern NBlock* programBlock;
extern int yyparse();

extern FILE *yyin;

int main(int argc, char **argv){
	if(argc!=2){
		cout<<"Usage: "<<argv[0]<<" <vtl source file>, "<<argc-1<<" arguments inputted instead\n";
		return 0;
	}
	//Point yyin to the source file
	yyin=fopen(argv[1], "r");
	
	//Parse
	cout<<"Parsing file "<<argv[1]<<"...\n";
	yyparse();

	//Print the resulting AST
	programBlock->print(0);
	return 0;
}
