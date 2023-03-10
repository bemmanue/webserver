#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../config/ServerConfig.hpp"

class Request;
class Response;


class Connection {
private:
	int							_fd;
	unsigned short				_port;

	Response*					_response;
	Request*					_request;

	std::vector<ServerConfig>	_serverConfigs;

	std::string					_str;

public:
	Connection();
	explicit Connection(const std::vector<ServerConfig>& serverConfigs);
	~Connection();

	int				readRequest();
	int				sendResponse();
	ServerConfig*	matchServerConfig(const std::string& host);
	std::string		readChunk(size_t size);
	std::string		readLength(size_t size);
	std::string		readLine();
	void			setRequest(const std::string& request);
	void			handleRequest();

	size_t			getPort();

	void setFD(int fd);
	void setPort(unsigned short port);

	void setServerConfigs(std::vector<ServerConfig> &serverConfigs);
};

#include "../http/Response.hpp"
#include "../http/Request.hpp"

#endif //CLIENT_HPP
