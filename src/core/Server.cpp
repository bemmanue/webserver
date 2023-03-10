#include "Server.hpp"

Server::Server() {

}

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
	unsigned short port;

	port = config.getPort();
	_servers[port].push_back(config);
}

int createSocket(int port) {
	struct sockaddr_in6   addr;
	int	rc;
	int	on = 1;

	int listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sd < 0) {
		perror("socket() failed");
		exit(-1);
	}

	rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0) {
		perror("setsockopt() failed");
		close(listen_sd);
		exit(-1);
	}

	rc = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (rc < 0) {
		perror("ioctl() failed");
		close(listen_sd);
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(port);

	rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
		perror("bind() failed");
		close(listen_sd);
		exit(-1);
	}

	rc = listen(listen_sd, 32);
	if (rc < 0) {
		perror("listen() failed");
		close(listen_sd);
		exit(-1);
	}
	return listen_sd;
}

void Server::start() {
	int		rc;
	int		timeout;

	timeout = (3 * 60 * 1000);

	//create server sockets
	for (auto & _server : _servers) {
		struct pollfd pollfd = {};

		pollfd.fd = createSocket(_server.first);
		pollfd.events = POLLIN;

		_poll_fds.push_back(pollfd);
		_server_fds.push_back(pollfd);
		std::cout << "started server on port " << _server.first << std::endl;
	}

	// start
	while (true) {
		std::cout << "waiting poll()" << std::endl;
		rc = poll(_poll_fds.data(), _poll_fds.size(), -1);
		if (rc < 0) {
			perror("poll() failed");
			break;
		} else if (rc == 0) {
			printf("poll() timed out\n");
			break;
		} else {
			process();
		}
	}

	//close fds
	for (int i = 0; i < _poll_fds.size(); i++) {
		if (_poll_fds[i].fd >= 0) {
			close(_poll_fds[i].fd);
		}
	}
}

void Server::process() {
	for (int id = 0; id < _poll_fds.size(); id++) {
		const int fd = _poll_fds[id].fd;

		if (fd < 0 || _poll_fds[id].revents & POLLNVAL) {
			continue;
		}

		if (isServerFD(id)) {
			if (_poll_fds[id].revents & POLLIN) {
				connect(_poll_fds[id].fd);
			}
		} else {
			if (_poll_fds[id].revents & POLLIN) {
				pollin(id);
			}
//			else if (_poll_fds[id].revents & POLLOUT) {
//				std::cout << "pollout" << std::endl;
//				pollout(id);
//			}
		}
	}
}

bool Server::isServerFD(int id) {
	if (id < _server_fds.size()) {
		return true;
	}
	return false;
}

void Server::connect(int fd) {
	int new_sd;

	printf("Listening socket is readable %d\n", fd);

	new_sd = accept(fd, nullptr, nullptr);
	if (new_sd < 0) {
		perror("connect() failed");
	}

	printf("New incoming connection - %d\n", new_sd);
	struct pollfd pollfd = {0, 0, 0};
	pollfd.fd = new_sd;
	pollfd.events = POLLIN;
	_poll_fds.push_back(pollfd);
}

void Server::pollin(int id) {
	int		rc;
	char	buffer[1000];

	printf("Descriptor %d is readable\n", _poll_fds[id].fd);

	// get request
	rc = recv(_poll_fds[id].fd, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("recv() failed");
	}

	if (rc == 0) {
		printf("Connection closed\n");
		close(_poll_fds[id].fd);
		_poll_fds[id].fd = -1;
		_poll_fds.erase(_poll_fds.begin() + id);
		return;
	}

	printf("%d bytes received\n", rc);

	// send response
	rc = send(_poll_fds[id].fd,
			  "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: 20\r\n\r\nhello from webserver", 91,
			  0);
	if (rc < 0) {
		perror("send() failed");
		close(_poll_fds[id].fd);
		_poll_fds[id].fd = -1;
		_poll_fds.erase(_poll_fds.begin() + id);
	}
}

void Server::pollout(int id) {

}
