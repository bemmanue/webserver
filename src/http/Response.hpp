#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/file.h>

#include "../config/ServerBlock.hpp"
#include "Request.hpp"


class Response {
private:
	Request			_request;
	ServerBlock		_serverConfig;

	std::string		_version;
	size_t			_status;
	std::string		_host;
	size_t			_length;
	bool			_chunked;
	std::string		_body;

public:
	Response();
	Response(const ServerBlock& serverConfig, const Request& request);
	~Response();

	std::string		toString();

	void	setStatus(size_t status);
	void	setContentLength(size_t length);
	void	setBody(const std::string& body);

private:
	void	handleRequest();
	void	handleGetRequest();
	void	handlePostRequest();
	void	handleDeleteRequest();

	void	makeResponseForFile(const std::string& requestTarget);
	void	makeResponseForDir(const std::string& requestTarget);

};


#endif //RESPONSE_HPP
