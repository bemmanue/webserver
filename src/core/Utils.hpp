#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <fstream>


std::string	capitalize(const std::string& str);
std::string toHexString(int num);

bool		resourceExists(const std::string& filename);
bool		isFile(const std::string& filename);
bool		isDirectory(const std::string& dirname);

std::string timeToString(std::filesystem::file_time_type point);

template <typename TP>
std::time_t to_time_t(TP tp);


#endif //UTILS_HPP
