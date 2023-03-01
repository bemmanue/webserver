#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sys/stat.h>

int			toDigit(char c);
std::string	capitalize(const std::string& str);

bool		resourceExists(const std::string& filename);
bool		isFile(const std::string& filename);
bool		isDirectory(const std::string& dirname);


#endif //UTILS_HPP
