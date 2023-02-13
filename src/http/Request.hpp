#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>

#include "Utils.hpp"
#include "Status.hpp"
#include "../config/ServerConfig.hpp"

#define CONTENT_LENGTH		"Content-Length"
#define HOST				"Host"
#define TRANSFER_ENCODING	"Transfer-Encoding"

enum State {
	REQUEST_LINE,
	HEADER_FIELD,
	BODY,
	FORMED
};

class Request {
private:
	unsigned short							_majorVersion;
	unsigned short							_minorVersion;

	std::string								_method;
	std::string								_requestTarget;
	std::string								_query;

	std::multimap<std::string, std::string>	_headers;

	std::string								_host;
	size_t									_length;
	bool									_chunked;

	std::string								_body;
	size_t									_status;

	ServerConfig*							_serverConfig;
	State									_state;

public:
	explicit Request(const std::string& request);
	Request(const Request& other);
	Request& operator=(const Request& other);
	~Request();

	void	setMethod(const std::string& method);
	void	setURI(const std::string& uri);
	void	setQuery(const std::string& query);
	void	setMajorVersion(unsigned short majorVersion);
	void	setMinorVersion(unsigned short minorVersion);
	void	setHeaderField(const std::string& name, const std::string& value);
	void	setHost(const std::string& value);
	void	setTransferEncoding(const std::string& value);
	void	setContentLength(const std::string& value);
	void	setBody(const std::string& body);
	void	setStatus(size_t status);
	void	setServerConfig(ServerConfig* serverConfig);

	std::string		getMethod() const;
	std::string		getRequestTarget() const;
	std::string		getQuery() const;
	size_t			getMajorVersion() const;
	size_t			getMinorVersion() const;
	std::string		getHost() const;
	std::string		getBody() const;
	size_t			getContentLength() const;
	size_t			getStatus() const;

	bool			isChunked() const;


friend std::ostream& operator<<(std::ostream& out, Request& re) {
	out << "Method: " << re.getMethod() << std::endl;
	out << "URI: " << re.getRequestTarget() << std::endl;
	out << "Query: " <<  re.getQuery() << std::endl;
	out << "Version: HTTP/" << re.getMajorVersion() << "." << re.getMinorVersion() << std::endl;
	out << "Host: " << re.getHost() << std::endl;
	out << "Transfer Encoding: " << std::endl;
	out <<  re.getBody() << std::endl;
	out <<  "Status: " << re.getStatus() << std::endl;
	return out;
}

private:
	void	parseRequest(const std::string& request);
	int		parseRequestLine(const std::string& request, size_t* pos);
	int		parseHeaderField(const std::string& str, size_t* i);
	void	parseBody(const std::string& request, size_t* pos);
};


#endif //REQUEST_HPP
