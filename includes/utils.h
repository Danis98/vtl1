#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <sstream>

template <typename T>
inline std::string to_string (T num){
	std::stringstream ss;
	ss << num;
	std::string n=ss.str();
	return n;
}

inline void fatal(std::string msg){
	std::cout<<msg;
	exit(0);
}

#endif
