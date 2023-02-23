#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <iostream>
#include <map>


class HeaderValue {};

template <typename T>
class MyHeaderValue: public HeaderValue{
	T	_value;
};

class Headers {
private:
	std::map<std::string, HeaderValue*>	_headers;
public:

};


#endif //HEADERS_HPP
