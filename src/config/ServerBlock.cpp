#include "../../include/config/ServerBlock.hpp"

bool isHostAddr(const std::string& addr) {
	in_addr_t		in_addr;
	struct in_addr	ip;
	hostent*		hostnames;

	in_addr = inet_addr(addr.c_str());
	if (in_addr == INADDR_NONE) {
		return false;
	}

	ip.s_addr = in_addr;

	hostnames = gethostbyaddr(&ip, sizeof(ip), AF_INET);
	if (hostnames == nullptr|| hostnames[0].h_name == nullptr) {
		return false;
	}
	return true;
}

bool isHostName(const std::string& addr) {
	if (!gethostbyname(addr.c_str())) {
		return false;
	}
	return true;
}

void ServerBlock::setHost(const std::string& addr) {
	if (isHostAddr(addr) || isHostName(addr)) {
		_addr = addr;
	} else {
		throw std::exception();
	}
}

void ServerBlock::setPort(size_t port) {
	if (port > 65535) {
		throw std::exception();
	}
	_port = port;
}

void ServerBlock::setServerName(const std::vector<std::string>& value) {
//	_server_name = parameter;
}

void ServerBlock::setErrorPages(const std::vector<std::string>& value) {
	_error_pages = value;
}

void ServerBlock::setClientMaxBodySize(const std::vector<std::string>& value) {
//	_client_max_body_size = parameter;
}

void ServerBlock::setLocation(const LocationBlock& parameter) {
	_location.push_back(parameter);
}

void ServerBlock::print() {
	std::cout << "port: " << _port << std::endl;
	std::cout << "addr: " << _addr << std::endl;
	std::cout << "server_name: " << _server_name << std::endl << "error_pages: ";

	for (int i = 0; i < _error_pages.size(); ++i) {
		std::cout << _error_pages[i] << " ";
	}

	std::cout << std::endl << "client_max_body_size: " << _client_max_body_size << std::endl;

	for (int i = 0; i < _location.size(); ++i) {
		std::cout << "path: " << _location[i]._path << std::endl;
		std::cout << "autoindex: " << _location[i]._autoindex << std::endl;

		std::cout << "methods_allowed: " << std::endl;
		std::cout << "CGIs: " << std::endl;
		for (int i = 0; i < _location[i]._methods_allowed.size(); ++i) {
			std::cout << _location[i]._methods_allowed[i] << " ";
		}
		std::cout << std::endl;

		std::cout << std::endl;
		std::cout << "root: " << _location[i]._root << std::endl;
	}
	std::cout << std::endl;
}