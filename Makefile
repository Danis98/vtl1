CPPFLAGS =-std=c++11

OBJ_FILES_GEN := parser.o tokens.o
OBJ_FILES := $(OBJ_FILES_GEN) main.o print_funcs.o

all: parser

parser.cpp: parser.y
	bison -d -o $@ $< -v

parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	lex -o $@ $<

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

parser: $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES)

clean:
	rm -rf $(OBJ_FILES_GEN) tokens.cpp parser.cpp parser.hpp *.o *~ parser parser.output

