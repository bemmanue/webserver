#include "Server.hpp"

Server::Server() {

}

Server::Server(const Server &other) {
	operator=(other);
}

Server &Server::operator=(const Server &other) {
	if (this != &other) {

	}
	return *this;
}

Server::~Server() = default;

void Server::setConfig(const ServerConfig &config) {

}

void Server::start() {

}
