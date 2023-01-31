#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>

#include "StatusCode.hpp"

//#define SP		" "
//#define HTAB	"\t"
//#define CRLF	"\r\n"
//#define COLON	":"
//#define MAX_METHOD_LENGTH	6

class Request {
public:
	Request();
	explicit Request(const std::string& request);
	~Request();

	void	setMethod(const std::string& method);
	void	setURI(const std::string& uri);
	void	setQuery(const std::string& query);
	void	setVersion(const std::string& version);
	void	setHeaderField(const std::string& fieldName, const std::string& fieldValue);
	void	setBody(const std::string& body);
	void	setStatus(size_t status);

private:
	std::string							_method;
	std::string							_uri;
	std::string							_query;
	std::string							_version;
	std::map<std::string, std::string>	_headerFields;
	std::string 						_body;
	size_t								_status;

	void		parseRequestLine(const std::string& request, size_t* pos);
	void		parseMethod(const std::string& request, size_t* pos);
	void		parseURI(const std::string& request, size_t* pos);
	void		parseVersion(const std::string& request, size_t* pos);
	void		parseHeaderFields(const std::string& request, size_t* pos);
	void		parseBody(const std::string& request, size_t* pos);
	void		skipSpace(const std::string& request, size_t* pos);

	std::string	getToken(const std::string& request, size_t pos);
	std::string	getSegment(const std::string& request, size_t pos);
	std::string	getAbsolutePath(const std::string& request, size_t pos);
	std::string	getQuery(const std::string& request, size_t pos);

	bool		isTchar(char a);
	bool		isPchar(char a);
	bool		isUnreserved(char a);
	bool		isPctEncoded(const std::string& str);
	bool		isSubDelim(char a);
};

#endif //REQUEST_HPP
