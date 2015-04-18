#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>
#include <fstream>

using namespace std;

#define OUT_FILE "vtl.out"

//Print on console
extern bool use_stdio;

extern streambuf *coutbuf;

void set_cout();
void reset_cout();

#endif
