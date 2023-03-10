#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <sys/stat.h>

std::string	capitalize(const std::string& str);
std::string toHexString(int num);

bool		resourceExists(const std::string& filename);
bool		isFile(const std::string& filename);
bool		isDirectory(const std::string& dirname);


#endif //UTILS_HPP
