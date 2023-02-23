#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>
#include <list>

#include "URI.hpp"
#include "Utils.hpp"
#include "Status.hpp"
#include "Headers.hpp"
#include "../config/ServerConfig.hpp"

#define HOST				"Host"
#define CONTENT_LENGTH		"Content-Length"
#define TRANSFER_ENCODING	"Transfer-Encoding"

enum State {
	requestLine,
	headerField,
	body
};

class Client;

class Request {
private:
	std::string			_method;
	std::string     	_requestTarget;
	unsigned short		_majorVersion;
	unsigned short		_minorVersion;
	Headers				_headers;
	std::string			_body;

	URI           		_host;
	size_t				_contentLength;
	bool				_chunked;
	
	size_t				_status;
	State				_state;
	bool 				_formed;

	ServerConfig*		_serverConfig;
	LocationConfig*		_locationConfig;

	Client*				_client;

public:
	Request(Client* client, const std::string& request);
	Request(const Request& other);
	Request& operator=(const Request& other);
	~Request();

	void	setMethod(const std::string& method);
	void	setRequestTarget(const std::string& requestTarget);
	void	setVersion(const std::string& version);
	void	setMajorVersion(unsigned short majorVersion);
	void	setMinorVersion(unsigned short minorVersion);
	void	setHeader(const std::string& name, const std::string& value);
	void	setHost(const std::string& value);
	void	setContentLength(const std::string& value);
	void	setTransferEncoding(const std::string& value);
	void	setBody(const std::string& body);
	void	setStatus(size_t status);
	void	setServerConfig(ServerConfig* serverConfig);
	void	setLocationConfig(LocationConfig* locationConfig);

	std::string		getMethod() const;
	std::string		getRequestTarget() const;
	size_t			getMajorVersion() const;
	size_t			getMinorVersion() const;
	URI				getHost() const;
	std::string		getBody() const;
	size_t			getContentLength() const;
	size_t			getStatus() const;
	bool			isChunked() const;
	bool			isFormed() const;

	void			isFormed(bool);

private:
	void	parseRequest(const std::string& request);
	void	parseRequestLine(const std::string& request, size_t* pos);
	void	parseHeaderField(const std::string& str, size_t* i);
	void	parseBody(const std::string& request, size_t* pos);
	void	parseChunkedBody(const std::string& request, size_t* pos);

public:
friend std::ostream& operator<<(std::ostream& out, Request& re) {
	out << "Method: " << re.getMethod() << std::endl;
	out << "Request target: " << re.getRequestTarget() << std::endl;
	out << "Version: HTTP/" << re.getMajorVersion() << "." << re.getMinorVersion() << std::endl;
//	out << "Host: " << re.getHost()._host << std::endl;
	out << "Content-Length: " << re.getContentLength() << std::endl;
	out << "Chunked: " << std::boolalpha << re.isChunked() << std::endl;
	out <<  re.getBody() << std::endl;
	out <<  "Status: " << re.getStatus() << std::endl;
//	out << *re._serverConfig << std::endl;
//	out << *re._locationConfig << std::endl;
	return out;
}

};


#include "../core/Client.hpp"

#endif //REQUEST_HPP
