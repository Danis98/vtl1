#################################################
#		VARIABLE DEFINITIONS		#
#################################################

CPPFLAGS =-std=c++11
INCLUDES =./includes/

#VTL1 front-end directory
FRONT_DIR := ./front
#VTL1 back-end directory
BACK_DIR := ./back/i368

#front-end object files
OBJ_FILES_FRONT := 		\
$(FRONT_DIR)/parser.o 		\
$(FRONT_DIR)/tokens.o 		\
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

#back-end object files
OBJ_FILES_BACK := 

OBJ_FILES := $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE) $(OBJ_FILES_BACK)

#################################################
#		COMMANDS				#
#################################################

LEX_PRG = 

ifeq ($(OS), Windows_NT)
	LEX_PRG := flex
else
	LEX_PRG := lex
endif

#################################################
#		COMPILATION RULES		#
#################################################

all: $(info OS: $(OS)) vtl

vtl: front_end middle_end back_end
	g++ -o $@ $(OBJ_FILES)

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

#Back-end compilation
back_end: $(OBJ_FILES_BACK)

#Cleaning rules
clean: clean_tmp
	rm -f vtl $(FRONT_DIR)/parser.output

clean_tmp:
	rm -rf $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE) $(GEN_FILES_CLEANUP) *~

