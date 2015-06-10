#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

enum data_type{
	VOID,
	BOOLEAN,
	INT,
	DOUBLE,
	STRING
};

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
