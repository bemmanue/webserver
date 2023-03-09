#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <ctime>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <cstring>

#include "../config/ServerConfig.hpp"
#include "Client.hpp"


class Server {
private:
	std::map<unsigned short, std::vector<ServerConfig>>	_servers;
	std::vector<struct pollfd>							_poll_fds;
	std::vector<struct pollfd>							_server_fds;


public:
	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);
	~Server();

	void	setConfig(const ServerConfig& config);

	void	start();
	void	process();
	void	connect(int fd);

	bool isServerFD(int fd);
};


#endif //SERVER_HPP
