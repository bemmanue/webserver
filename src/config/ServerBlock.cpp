#include "ServerBlock.hpp"

ServerBlock::ServerBlock():
	_majorVersion(1),
	_minorVersion(1),
	_host(DEFAULT_HOST),
	_port(DEFAULT_PORT),
	_client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE) {
	_methods_allowed.insert(GET);
	_methods_allowed.insert(POST);
	_methods_allowed.insert(DELETE);
}

ServerBlock::ServerBlock(const ServerBlock &other) {
	operator=(other);
}

ServerBlock& ServerBlock::operator=(const ServerBlock &other) {
	if (this != &other) {
		_majorVersion = other._majorVersion;
		_minorVersion = other._minorVersion;
		_host = other._host;
		_port = other._port;
		_server_names = other._server_names;
		_error_pages = other._error_pages;
		_client_max_body_size = other._client_max_body_size;
		_locations = other._locations;
		_methods_allowed = other._methods_allowed;
	}
	return *this;
}

ServerBlock::~ServerBlock() {
}

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
		_host = addr;
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
	_server_names.insert(name);
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

void ServerBlock::setLocation(const std::string& path, const LocationBlock& location) {
	_locations[path] = location;
}

const std::string& ServerBlock::getHost() const {
	return _host;
}

size_t ServerBlock::getPort() const {
	return _port;
}

size_t ServerBlock::getMajorVersion() const {
	return _majorVersion;
}

size_t ServerBlock::getMinorVersion() const {
	return _minorVersion;
}

const std::set<std::string>& ServerBlock::getServerNames() const {
	return _server_names;
}

const std::map<int, std::string>& ServerBlock::getErrorPages() const {
	return _error_pages;
}

uint64_t ServerBlock::getClientMaxBodySize() const {
	return _client_max_body_size;
}

const std::map<std::string, LocationBlock>& ServerBlock::getLocations() const {
	return _locations;
}

bool ServerBlock::hasLocation(const std::string &location) {
	return false;
}

bool ServerBlock::isMethodAllowed(const std::string &method) {
	std::set<std::string>::const_iterator pos = _methods_allowed.find(method);
	if (pos == _methods_allowed.end()) {
		return false;
	}
	return true;
}
