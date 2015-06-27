#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

#define MAX_STRLEN 256

enum data_type{
	VOID,
	BOOLEAN,
	INT,
	DOUBLE,
	STRING
};

inline int get_width(enum data_type dt){
	switch(dt){
		case VOID:
			return 0;
		case BOOLEAN:
			return 4;
		case INT:
			return 4;
		case DOUBLE:
			return 8;
		case STRING:
			return 4*MAX_STRLEN;
		default:
			return 0;
	}
}

inline std::string data_type_names(enum data_type dt){
	switch(dt){
		case VOID:
			return "VOID";
		case BOOLEAN:
			return "BOOLEAN";
		case INT:
			return "INT";
		case DOUBLE:
			return "DOUBLE";
		case STRING:
			return "STRING";
		default:
			return "UNDEFINED";
	}
}


#endif
