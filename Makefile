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
$(FRONT_DIR)/print_funcs.o

GEN_FILES_CLEANUP := 		\
$(FRONT_DIR)/tokens.cpp 	\
$(FRONT_DIR)/parser.cpp		\
$(FRONT_DIR)/parser.hpp

#middle-end object files
OBJ_FILES_MIDDLE := 	\
main.o			\
s_table.o		\
typecheck.o

#back-end object files
OBJ_FILES_BACK := 

OBJ_FILES := $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE) $(OBJ_FILES_BACK)

#################################################
#		COMPILATION RULES		#
#################################################

all: vtl

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
	lex -o $@ $<

#Middle-end compilation
middle_end: $(OBJ_FILES_MIDDLE)

#Back-end compilation
back_end: $(OBJ_FILES_BACK)

#Cleaning rules
clean: clean_tmp
	rm -f vtl $(FRONT_DIR)/parser.output

clean_tmp:
	rm -rf $(OBJ_FILES_FRONT) $(OBJ_FILES_MIDDLE) $(GEN_FILES_CLEANUP) *~

