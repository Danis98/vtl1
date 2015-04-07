#include <iostream>
#include "node.h"

extern NBlock* programBlock;
extern int yyparse();

int main(int argc, char **argv){
	yyparse();
	printf("Parsing!\n");
	programBlock->print();
	return 0;
}
