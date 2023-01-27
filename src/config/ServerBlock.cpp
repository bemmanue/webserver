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

void ServerBlock::setServerName(const std::string& name) {
	_server_name.insert(name);
}

void ServerBlock::setErrorPages(int code, const std::string& path) {
	if (code < 300 || code > 599) {
		throw std::exception();
	}
	_error_pages[code] = path;
}

void ServerBlock::setClientMaxBodySize(uint64_t bytes) {
	_client_max_body_size = bytes;
}

void ServerBlock::setLocation(const LocationBlock& parameter) {
	_location.push_back(parameter);
}

void ServerBlock::print() {
	std::cout << "\t" << "addr: " << _addr << std::endl;
	std::cout << "\t" << "port: " << _port << std::endl;

	for (std::set<std::string>::iterator i = _server_name.begin(); i != _server_name.end(); i++) {
		std::cout << "\t" << "server_name: " << *i << std::endl;
	}

	for (std::map<int, std::string>::iterator i = _error_pages.begin(); i != _error_pages.end(); i++) {
		std::cout << "\t" << "error_page: " << (*i).first << " " << (*i).second << std::endl;
	}

	std::cout << "\t" << "client_max_body_size: " << _client_max_body_size << std::endl;

	for (int i = 0; i < _location.size(); ++i) {
		std::cout << "\t" << "Location №" << (i + 1) << std::endl;
		_location[i].print();
	}
}