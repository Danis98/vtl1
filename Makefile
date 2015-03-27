FLAGS := -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
CPP_FILES_GEN := parser.cpp tokens.cpp
CPP_FILES := $(CPP_FILES_GEN) main.cpp codeGen.cpp

all: parser

parser.cpp: parser.y
	bison -d -o $@ $<

parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	lex -o $@ $<

parser: $(CPP_FILES)
	g++ -o $@ $(FLAGS) $(CPP_FILES)

clean:
	rm $(CPP_FILES_GEN) parser.hpp

