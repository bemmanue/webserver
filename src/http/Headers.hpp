#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <iostream>
#include <map>
#include <list>
#include <any>

#include "Status.hpp"
#include "URI.hpp"

#define HOST				"Host"
#define CONTENT_LENGTH		"Content-Length"
#define TRANSFER_ENCODING	"Transfer-Encoding"


class Headers {
private:
	std::map<std::string, std::any>	_headers;
	size_t							_status;
public:
	Headers();
	Headers(const Headers& other);
	Headers& operator=(const Headers& other);
	~Headers();

	std::any	operator[](const std::string& headerName);

	void	setHeader(const std::string& headerName, const std::string& headerValue);
	void	setHost(const std::string& headerValue);
	void	setContentLength(const std::string& headerValue);
	void	setTransferEncoding(const std::string& headerValue);

	bool	hasHeader(const std::string& name);

	size_t	getStatus();
};


#endif //HEADERS_HPP
