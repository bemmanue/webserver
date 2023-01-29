#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>

#define REQUEST_LINE_LENGTH 8000

class Request {
public:
	struct RequestLine {
		std::string	_method;
		std::string	_uri;
		std::string	_version;
	};

private:
	RequestLine							_requestLine;
	std::map<std::string, std::string>	_headerFields;
	std::string 						_body;

public:
	void	setRequestLine(const RequestLine& requestLine);
	void	setHeaderField(const std::string& fieldName, const std::string& fieldValue);
	void	setBody(const std::string& body);
};

Request parseRequest(std::string& str);

#endif //REQUEST_HPP
