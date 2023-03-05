#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"


class Server {
private:

public:
	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);
	~Server();

	void	setConfig(const ServerConfig& config);

	void	start();
};


#endif //SERVER_HPP
