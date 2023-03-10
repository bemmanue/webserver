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
#include "Connection.hpp"


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

	int 	createServerSocket(int port);
	void 	createServerPollFDs();
	void 	closeSockets();

	void	start();
	void	process();
	void	acceptClient(int fd);
	void	pollin(int id);
};


#endif //SERVER_HPP
