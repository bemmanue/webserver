#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <stack>
#include <any>

#include "URI.hpp"
#include "Utils.hpp"
#include "Status.hpp"
#include "../config/ServerConfig.hpp"

#define HOST				"Host"
#define CONTENT_LENGTH		"Content-Length"
#define TRANSFER_ENCODING	"Transfer-Encoding"

enum ParsingState {
	PARSING_REQUEST_LINE,
	PARSING_HEADERS,
	PARSING_BODY_BY_LENGTH,
	PARSING_CHUNK_SIZE,
	PARSING_CHUNK_DATA,
	PARSING_TRAILER_PART
};

class Client;

class Request {
private:
	std::string						_method;
	std::string     				_requestTarget;
	unsigned short					_majorVersion;
	unsigned short					_minorVersion;
	std::map<std::string, std::any>	_headers;
	std::string						_body;
	
	size_t							_status;
	ParsingState					_state;
	bool 							_formed;
	size_t							_expectedBodySize;

	ServerConfig*					_serverConfig;
	LocationConfig*					_locationConfig;
	Client*							_client;

public:
	Request(Client* client);
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
	void	setTransferEncoding(const std::string& value);
	void	setContentLength(const std::string& value);
	void	setBody(const std::string& body);
	void	setServerConfig(ServerConfig* serverConfig);
	void	setLocationConfig(LocationConfig* locationConfig);

	[[nodiscard]] std::string		getMethod() const;
	[[nodiscard]] std::string		getRequestTarget() const;
	[[nodiscard]] size_t			getMajorVersion() const;
	[[nodiscard]] size_t			getMinorVersion() const;
	URI								getHost();
	std::stack<std::string>			getTransferEncoding();
	size_t							getContentLength();
	[[nodiscard]] std::string		getBody() const;
	[[nodiscard]] size_t			getStatus() const;
	[[nodiscard]] ServerConfig*		getServerConfig() const;
	[[nodiscard]] LocationConfig*	getLocationConfig() const;
	[[nodiscard]] ParsingState		getParsingState() const;
	[[nodiscard]] size_t			getExpectedBodySize() const;

	bool	isFormed() const;

	void	isFormed(bool);
	bool	hasHeader(const std::string& headerName);

	void	parseLine(const std::string& line);
	void	parseRequestLine(const std::string& line);
	void	parseHeaderField(const std::string& line);
	void	parseChunkSize(const std::string& line);
	void	parseChunkData(const std::string& line);
	void	parseTrailerPart(const std::string& line);
	void	parseBody(const std::string& line);
	void	parseChunkedBody(const std::string& line);

public:
friend std::ostream& operator<<(std::ostream& out, Request& re) {
	out << "Method: " << re.getMethod() << std::endl;
	out << "Request target: " << re.getRequestTarget() << std::endl;
	out << "Version: HTTP/" << re.getMajorVersion() << "." << re.getMinorVersion() << std::endl;
	out << "Host: " << re.getHost().getAuthority() << std::endl;
	out << "Transfer-Encoding: " << re.getTransferEncoding().top() << std::endl;
	out << "Content-Length: " << re.getContentLength() << std::endl;
	out <<  re.getBody() << std::endl;
	out <<  "Status: " << re.getStatus() << std::endl;
//	out << *re._serverConfig << std::endl;
//	out << *re._locationConfig << std::endl;
	return out;
}

};


#include "../core/Client.hpp"

#endif //REQUEST_HPP
