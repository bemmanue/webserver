#include "Server.hpp"

Server::Server() {}

Server::Server(const Server &other) {
	operator=(other);
}

Server &Server::operator=(const Server &other) {
	if (this != &other) {
		_servers = other._servers;
		_poll_fds = other._poll_fds;
		_server_fds = other._server_fds;
	}
	return *this;
}

Server::~Server() = default;

void Server::setConfig(const ServerConfig& config) {
	_servers[config.getPort()].push_back(config);
}

void Server::start() {

	createServerPollFDs();

	while (true) {
		if (poll(_poll_fds.data(), _poll_fds.size(), -1) < 0) {
			perror("poll() failed");
			break;
		}
		process();
	}

	closeSockets();
}

void Server::process() {
	for (size_t id = 0; id < _poll_fds.size(); id++) {
		if (_poll_fds[id].revents & POLLIN) {
			if (id < _server_fds.size()) {
				acceptClient((int)id);
			} else {
				pollin((int)id);
			}
		}
	}
}

void Server::acceptClient(int id) {
	int	client_fd;
	int	server_fd = _poll_fds[id].fd;
	struct pollfd	pollfd = {};

	client_fd = accept(server_fd, nullptr, nullptr);
	if (client_fd < 0) {
		perror("acceptClient() failed");
	}

	pollfd.fd = client_fd;
	pollfd.events = POLLIN;

	_poll_fds.push_back(pollfd);
}

void Server::pollin(int id) {
	Connection	connection;

	connection.setFD(_poll_fds[id].fd);
	connection.setPort(8888);
	connection.setServerConfigs(_servers[8888]);

	if (!connection.readRequest()) {
		printf("Connection closed\n");
		close(_poll_fds[id].fd);
		_poll_fds.erase(_poll_fds.begin() + id);
		return;
	}

	if (connection.sendResponse()) {
		perror("send() failed");
		close(_poll_fds[id].fd);
		_poll_fds.erase(_poll_fds.begin() + id);
	}
}

void Server::createServerPollFDs() {
	int				socket;
	struct pollfd	pollfd;

	for (auto & _server : _servers) {
		socket = createServerSocket(_server.first);

		bzero(&pollfd, sizeof(pollfd));
		pollfd.fd = socket;
		pollfd.events = POLLIN;

		_poll_fds.push_back(pollfd);
		_server_fds.push_back(pollfd);
	}
}

int Server::createServerSocket(int port) {
	struct sockaddr_in6	addr;
	int	sock;
	int	on = 1;

	if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("socket() failed");
		exit(EXIT_FAILURE);
	}

	if ((setsockopt(sock, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(on))) < 0) {
		perror("setsockopt() failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	if ((ioctl(sock, FIONBIO, &on)) < 0) {
		perror("ioctl() failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(port);

	if ((bind(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
		perror("bind() failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	if ((listen(sock, 32)) < 0) {
		perror("listen() failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	return sock;
}

void Server::closeSockets() {
	for (auto & _poll_fd : _poll_fds) {
		if (_poll_fd.fd >= 0) {
			close(_poll_fd.fd);
		}
	}
}
