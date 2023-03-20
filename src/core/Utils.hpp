#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <fstream>


#define HTML_BEG "<html>"
#define HTML_END "</html>"

#define HEAD_BEG "<head>"
#define HEAD_END "</head>"

#define TITLE_BEG "<title>"
#define TITLE_END "</title>"

#define BODY_BEG "<body>"
#define BODY_END "</body>"

#define H1_BEG "<h1>"
#define H1_END "</h1>"

#define HR_BEG "<hr>"
#define HR_END "</hr>"

#define HR_BEG "<hr>"
#define HR_END "</hr>"

#define PRE_BEG "<pre>"
#define PRE_END "</pre>"

#define HREF_BEG "<a href=\""
#define HREF_END "\">"
#define A_END "</a>"


std::string	capitalize(const std::string& str);
std::string toHexString(int num);

bool		resourceExists(const std::string& filename);
bool		isFile(const std::string& filename);
bool		isDirectory(const std::string& dirname);

std::string timeToString(std::filesystem::file_time_type point);

template <typename TP>
std::time_t to_time_t(TP tp);


#endif //UTILS_HPP
