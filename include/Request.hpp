#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>

class Request {
private:
	std::string _method;
	std::string	_URI;
	std::string _version;

	std::string _body;
};

Request parseRequest(char *str);

#endif //REQUEST_HPP
