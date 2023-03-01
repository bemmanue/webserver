#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/file.h>
#include <sstream>

#include "../config/ServerConfig.hpp"
#include "Request.hpp"


class Response {
private:
	unsigned short					_majorVersion;
	unsigned short					_minorVersion;
	unsigned short					_status;
	std::string						_reasonPhrase;
	std::map<std::string, std::any>	_headers;
	std::string						_body;

	Request*						_request;

public:
	Response(Request* request);
	Response(const Response& other);
	Response& operator=(const Response& other);
	~Response();

	std::string		toString();

private:
	void	handleRequest();
	void	handleGetRequest();
	void	handlePostRequest();
	void	handleDeleteRequest();

	void	makeResponseForFile();
	void	makeResponseForError();
	void	makeResponseForDir();
	void	makeResponseForMethod();
};


#endif //RESPONSE_HPP
