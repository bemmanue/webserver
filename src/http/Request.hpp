#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>

#include "../config/Global.hpp"
#include "Utils.hpp"
#include "Status.hpp"

#define HOST				"Host"
#define TRANSFER_ENCODING	"Transfer-Encoding"
#define CONTENT_LENGTH		"Content-Length"


class RequestException : public std::exception {
public:
	explicit RequestException(size_t code): _code(code) {}
	virtual const char* what() const throw() { return statusLines[_code].c_str(); }
	size_t	getCode() const { return _code; }

private:
	size_t	_code;
};


class Request {
private:
	std::string		_method;
	std::string		_requestTarget;
	std::string		_query;
	std::string		_version;

	std::string		_host;
	size_t			_length;
	bool			_chunked;

	std::string		_body;
	size_t			_status;

public:
	Request();
	explicit Request(const std::string& request);
	~Request();

	std::string		getMethod() const;
	std::string		getRequestTarget() const;
	std::string		getQuery() const;
	std::string		getVersion() const;
	std::string		getHost() const;
	std::string		getBody() const;
	size_t			getContentLength() const;
	size_t			getStatus() const;

	bool			isChunked() const;

friend std::ostream& operator<<(std::ostream& out, Request& re) {
	out << "Method: " << re.getMethod() << std::endl;
	out << "URI: " << re.getRequestTarget() << std::endl;
	out << "Query: " <<  re.getQuery() << std::endl;
	out << "Version: " << re.getVersion() << std::endl;
	out << "Host: " << re.getHost() << std::endl;
	out << "Transfer Encoding: " << std::endl;
	out <<  re.getBody() << std::endl;
	out <<  "Status: " << re.getStatus() << std::endl;
	return out;
}

private:
	void	parseRequest(const std::string& request);
	void	parseRequestLine(const std::string& request, size_t* pos);
	void	parseHeaderFields(const std::string& request, size_t* pos);
	void	parseBody(const std::string& request, size_t* pos);

	void	setMethod(const std::string& method);
	void	setURI(const std::string& uri);
	void	setQuery(const std::string& query);
	void	setVersion(const std::string& version);
	void	setHeaderField(const std::string& name, const std::string& value);
	void	setHost(const std::string& value);
	void	setTransferEncoding(const std::string& value);
	void	setContentLength(const std::string& value);
	void	setBody(const std::string& body);
	void	setStatus(size_t status);
};


#endif //REQUEST_HPP
