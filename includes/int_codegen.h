#ifndef _CODE_GEN_H	
#define _CODE_GEN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <utils.h>

extern std::ofstream outfile;

enum int_ops{
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_ASSIGN,
	OP_PARAM,
	OP_CALL,
	OP_JUMP,
	OP_JUMPIF,
	OP_JUMPNIF,
	OP_LABEL,
	OP_CEQ,
	OP_CNE,
	OP_CLT,
	OP_CLE,
	OP_CGT,
	OP_CGE,
	OP_RET
};

extern std::string int_ops_str[];

typedef  std::string temp_var;
typedef std::string label;

struct quad{
	enum int_ops op;
	temp_var arg1, arg2, res;
};

extern int occupied_temps, occupied_labels;

temp_var get_temp();
label get_label();

struct intermediate_form{
	std::vector<quad> instructions;
	intermediate_form(){}
	void concat(struct intermediate_form a){
		instructions.insert(instructions.begin(),
					a.instructions.begin(),
					a.instructions.end());
	}
};

extern struct intermediate_form int_code;

inline void emit(enum int_ops op, temp_var a1, temp_var a2, temp_var r){
	if(op==OP_ASSIGN)
		outfile<<"\t"<<r<<" := "<<a1<<std::endl;
	else if(op==OP_PARAM)
		outfile<<"\t"<<int_ops_str[(int)op]<<" "<<a1<<std::endl;
	else if(op==OP_RET)
		outfile<<"\t"<<int_ops_str[(int)op]<<(a1==""?"":" "+a1)<<std::endl;
	else if(op==OP_LABEL)
		outfile<<a1<<":\n";
	else if(op==OP_CALL)
		outfile<<"\t"<<((r=="")?"":(r+" := "))<<int_ops_str[(int)op]<<" "<<a1<<" "<<a2<<std::endl;
	else if(op==OP_JUMP)
		outfile<<"\t"<<int_ops_str[(int)op]<<" "<<a1<<":\n";
	else if(op==OP_JUMPIF || op==OP_JUMPNIF)
		outfile<<"\t"<<int_ops_str[(int)op]<<" "<<a1<<" "<<a2<<std::endl;
	else
		outfile<<"\t"<<r<<" := "<<a1<<" "<<int_ops_str[(int)op]<<" "<<a2<<std::endl;
	int_code.instructions.push_back({op, a1, a2, r});
}

#endif
