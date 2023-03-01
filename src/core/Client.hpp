#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../config/ServerConfig.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"

class Client {
private:
	std::vector<ServerConfig>	_serverConfigs;
	std::string					_request;
	size_t 						_port;

public:
	explicit Client(const std::vector<ServerConfig>& serverConfigs);
	~Client();

	ServerConfig*	matchServerConfig(const std::string& host);
	std::string		readChunk(size_t size);
	std::string		readLength(size_t size);
	std::string		readLine();
	void			setRequest(const std::string& request);
	void			handleRequest();

	size_t			getPort();
};


#endif //CLIENT_HPP
