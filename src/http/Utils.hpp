#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sys/stat.h>

#ifndef HTTP_METHODS
	# define GET		"GET"
	# define POST		"POST"
	# define PUT		"PUT"
	# define HEAD		"HEAD"
	# define DELETE		"DELETE"
	# define CONNECT	"CONNECT"
	# define OPTIONS	"OPTIONS"
	# define TRACE		"TRACE"
	# define PATCH		"PATCH"
#endif

#define SP		" "
#define CRLF	"\r\n"

std::string	capitalize(const std::string& str);

std::string	readToken(const std::string& str, size_t* pos);
std::string	readVersion(const std::string& str, size_t* pos);
std::string	readQuery(const std::string& str, size_t* pos);
std::string	readSegment(const std::string& str, size_t* pos);
std::string	readFieldValue(const std::string& str, size_t* pos);
std::string	readAbsolutePath(const std::string& str, size_t* pos);
std::string	readChunkData(const std::string& str, size_t* pos, size_t chunkSize);
size_t		readChunkSize(const std::string& str, size_t* pos);

void		skipOWS(const std::string& str, size_t* pos);
void		skipCRLF(const std::string& str, size_t* pos);
void		skipRequiredChar(const std::string& str, size_t* pos, char a);

bool		isTchar(char a);
bool		isPchar(char a);
bool		isVchar(char a);
bool		isSubDelim(char a);
bool		isFieldVchar(char a);
bool		isUnreserved(char a);
bool		isPctEncoded(const std::string& str);
bool		isEmptyLine(const std::string& str, size_t pos);

bool		resourceExists(const std::string& filename);
bool		isFile(const std::string& filename);
bool		isDirectory(const std::string& dirname);

bool		isHTTPMethod(const std::string& method);

#endif //UTILS_HPP
