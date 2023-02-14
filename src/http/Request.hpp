#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>

#include "URI.hpp"
#include "Utils.hpp"
#include "Status.hpp"
#include "../config/ServerConfig.hpp"

#define CONTENT_LENGTH		"Content-Length"
#define HOST				"Host"
#define TRANSFER_ENCODING	"Transfer-Encoding"

enum State {
	requestLine,
	headerField,
	body
};

class Request {
private:
	std::string							_method;

	URI            						_requestTarget;
	URI           						_host;

	unsigned short						_majorVersion;
	unsigned short						_minorVersion;

	std::map<std::string, std::string>	_headers;

	size_t								_length;
	bool								_chunked;

	std::string							_body;
	size_t								_status;

	ServerConfig*						_serverConfig;
	State								_state;
	bool 								_formed;


public:
	explicit Request(const std::string& request);
	Request(const Request& other);
	Request& operator=(const Request& other);
	~Request();

	void	setMethod(const std::string& method);
	void	setRequestTarget(const URI& uri);
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
	URI				getRequestTarget() const;
	size_t			getMajorVersion() const;
	size_t			getMinorVersion() const;
	URI				getHost() const;
	std::string		getBody() const;
	size_t			getContentLength() const;
	size_t			getStatus() const;

	bool			isChunked() const;
	bool			isFormed() const;

	void			isFormed(bool);


friend std::ostream& operator<<(std::ostream& out, Request& re) {
	out << "Method: " << re.getMethod() << std::endl;
	out << "URI: " << re.getRequestTarget()._path << std::endl;
	out << "Version: HTTP/" << re.getMajorVersion() << "." << re.getMinorVersion() << std::endl;
	out << "Host: " << re.getHost()._host << std::endl;
	out << "Transfer Encoding: " << std::endl;
	out <<  re.getBody() << std::endl;
	out <<  "Status: " << re.getStatus() << std::endl;
	return out;
}

private:
	void	parseRequest(const std::string& request);
	void	parseRequestLine(const std::string& request, size_t* pos);
	void	parseHeaderField(const std::string& str, size_t* i);
	void	parseBody(const std::string& request, size_t* pos);
	void	parseChunkedBody(const std::string& request, size_t* pos);
};


#endif //REQUEST_HPP
