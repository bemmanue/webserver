#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <arpa/inet.h>


std::string	readToken(const std::string& str, size_t* pos);
std::string	readQuotedString(const std::string& str, size_t* pos);
std::string	readDecNum(const std::string& str, size_t* pos);
std::string	readVersion(const std::string& str, size_t* pos);
std::string	readQuery(const std::string& str, size_t* pos);
std::string	readFragment(const std::string& str, size_t* pos);
std::string	readWord(const std::string& str, size_t* pos);
std::string	readScheme(const std::string& str, size_t* pos);
std::string	readUserInfo(const std::string& str, size_t* pos);
std::string	readHost(const std::string& str, size_t* pos);
std::string readIPLiteral(const std::string& str, size_t* i);
std::string readIPv6address(const std::string& str, size_t* i);
std::string readIPvFuture(const std::string& str, size_t* i);
int			readPort(const std::string& str, size_t* pos);
std::string	readSegment(const std::string& str, size_t* pos);
std::string	readFieldValue(const std::string& str, size_t* pos);
std::string	readPathAbempty(const std::string& str, size_t* pos);
std::string	readPathAbsolute(const std::string& str, size_t* pos);
std::string	readPathRootless(const std::string& str, size_t* pos);
std::string	readChunkData(const std::string& str, size_t* pos, size_t chunkSize);
long 		readChunkSize(const std::string& str, size_t* pos);

void		skipOWS(const std::string& str, size_t* pos);
bool		skipCRLF(const std::string& str, size_t* pos);
bool		skipRequiredChar(const std::string& str, size_t* pos, char c);

bool		isTchar(char a);
bool		isPchar(char a);
bool		isVchar(char a);
bool		isGenDelim(char a);
bool		isSubDelim(char a);
bool		isFieldVchar(char a);
bool		isReserved(char a);
bool		isUnreserved(char a);
bool		isQuotedText(char a);
bool		isObsText(unsigned char a);
bool		isQuotedPair(const std::string& str);
bool		isPctEncoded(const std::string& str);
bool		isValidHost(const std::string& str);
bool		isIPLiteral(const std::string& str);
bool		isIPv4address(const std::string& str);
bool		isIPv6address(const std::string& str);
bool		isIPvFuture(const std::string& str);
bool		isRegName(const std::string& str);
bool		isOriginForm(const std::string& str);


#endif //UTILS_HPP
