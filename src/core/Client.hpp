#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../config/ServerConfig.hpp"


class Client {
private:
	std::vector<ServerConfig>	_serverConfigs;

public:
	explicit Client(const std::vector<ServerConfig>& serverConfigs);
	~Client();

	ServerConfig*	matchServerConfig(const std::string& host);
};


#endif //CLIENT_HPP
