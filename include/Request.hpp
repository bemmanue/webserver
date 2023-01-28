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


static Request parseRequest(std::string *str);

#endif //REQUEST_HPP
