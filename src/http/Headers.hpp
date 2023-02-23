#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <iostream>
#include <map>
#include <list>

#include "Status.hpp"
#include "URI.hpp"

#define HOST				"Host"
#define CONTENT_LENGTH		"Content-Length"
#define TRANSFER_ENCODING	"Transfer-Encoding"


class HeaderValueInterface {
	HeaderValueInterface() {};
};

template <typename T>
class HeaderValue: public HeaderValueInterface {
private:
	T		_value;
public:
	HeaderValue() {};

	bool	setValue(const T& value) { _value = value; }
	T&		getValue() { return _value; }
};

class Headers {
private:
	std::map<std::string, HeaderValueInterface*>	_headers;
	size_t											_status;
public:
	Headers();
	Headers(const Headers& other);
	Headers& operator=(const Headers& other);
	~Headers();

	HeaderValueInterface*	operator[](const std::string& headerName);

	void	setHeader(const std::string& headerName, const std::string& headerValue);
	void	setHost(const std::string& headerValue);
	void	setContentLength(const std::string& headerValue);
	void	setTransferEncoding(const std::string& headerValue);

	bool	hasHeader(const std::string& name);

	size_t	getStatus();
};


#endif //HEADERS_HPP
