#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <sstream>

inline std::string to_string (int num){
	std::stringstream ss;
	ss << num;
	std::string n=ss.str();
	return n;
}

#endif
