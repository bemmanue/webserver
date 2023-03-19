#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <fstream>
#include <unistd.h>
#include <sys/file.h>
#include <sstream>

#include "Pages.hpp"
#include "../core/Utils.hpp"
#include "Request.hpp"

class Response {
private:
	std::string							_target;
	unsigned short						_majorVersion;
	unsigned short						_minorVersion;
	Status								_status;
	std::map<std::string, std::string>	_headers;
	std::string							_body;

	LocationConfig*						_locationConfig;
	ServerConfig*						_serverConfig;
	Request*							_request;

public:
	Response(Request* request);
	Response(const Response& other);
	Response& operator=(const Response& other);
	~Response();

	void	setStatus(Status status);

	std::string		getResponse();
	Status			getStatus();
	std::string		getStatusLine();
	std::string		getVersion();
	std::string		getBody();

private:
	void	handleRequest();
	void	handleMethod();

	void	Get();
	void	Post();
	void	Delete();

	void	makeResponseForFile();
	void	makeResponseForError();
	void	makeResponseForDir();
	void	makeResponseForListing();

	bool	setIndexFile();
	bool	setIndexDirectory();

	std::string timeToString(std::filesystem::file_time_type point);
};

#endif //RESPONSE_HPP


