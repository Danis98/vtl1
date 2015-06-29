#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <iostream>
#include <sstream>

template <typename T>
inline std::string to_string ( T Number )
{
	std::stringstream ss;
	ss << Number;
	return ss.str();
}

#endif