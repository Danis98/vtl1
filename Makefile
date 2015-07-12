#################################################
#		VARIABLE DEFINITIONS		#
#################################################

CPPFLAGS =-std=gnu++11
INCLUDES =./includes/

#VTL1 front-end directory
FRONT_DIR := ./front

#front-end object files
OBJ_FILES_FRONT := 		\
$(FRONT_DIR)/tokens.o 		\
$(FRONT_DIR)/parser.o 		\
$(FRONT_DIR)/print_funcs.o	\
$(FRONT_DIR)/s_table.o		\
$(FRONT_DIR)/typecheck.o	\
$(FRONT_DIR)/codegen.o

GEN_FILES_CLEANUP := 		\
$(FRONT_DIR)/tokens.cpp 	\
$(FRONT_DIR)/parser.cpp		\
$(FRONT_DIR)/parser.hpp

#middle-end object files
OBJ_FILES_MIDDLE := 	\
main.o

OBJ_FILES := $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE)

#################################################
#		COMMANDS				#
#################################################

ifeq ($(OS), Windows_NT)
	LEX_PRG := flex
	EXEC_EXTENSION := .exe
else
	LEX_PRG := lex
endif

#################################################
#		COMPILATION RULES		#
#################################################

all: vtl

vtl: front_end middle_end
	g++ -o $@$(EXEC_EXTENSION) $(OBJ_FILES)

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $< -I$(INCLUDES)

#Front-end compilation
front_end: $(OBJ_FILES_FRONT)

$(FRONT_DIR)/parser.cpp: $(FRONT_DIR)/parser.y
	bison -d -o $@ $< -v

$(FRONT_DIR)/parser.hpp: $(FRONT_DIR)/parser.cpp

$(FRONT_DIR)/tokens.cpp: $(FRONT_DIR)/tokens.l $(FRONT_DIR)/parser.hpp
	$(LEX_PRG) -o $@ $<

#Middle-end compilation
middle_end: $(OBJ_FILES_MIDDLE)

#Cleaning rules
clean: clean_tmp clean_compiled
	rm -f vtl$(EXEC_EXTENSION) $(FRONT_DIR)/parser.output

clean_compiled:
	rm -rf examples/*.vvm examples/*.vvm-dbg

clean_tmp:
	rm -rf $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE) $(GEN_FILES_CLEANUP) *~

