#include "ServerConfig.hpp"

ServerConfig::ServerConfig():
		_majorVersion(1),
		_minorVersion(1),
		_host(DEFAULT_HOST),
		_port(DEFAULT_PORT),
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE) {
	_methodsAllowed.insert(GET);
	_methodsAllowed.insert(POST);
	_methodsAllowed.insert(DELETE);
}

ServerConfig::ServerConfig(const ServerConfig &other) {
	operator=(other);
}

ServerConfig& ServerConfig::operator=(const ServerConfig &other) {
	if (this != &other) {
		_majorVersion = other._majorVersion;
		_minorVersion = other._minorVersion;
		_host = other._host;
		_port = other._port;
		_serverNames = other._serverNames;
		_errorPages = other._errorPages;
		_clientMaxBodySize = other._clientMaxBodySize;
		_locations = other._locations;
		_methodsAllowed = other._methodsAllowed;
	}
	return *this;
}

ServerConfig::~ServerConfig() {
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

void ServerConfig::setHost(const std::string& addr) {
	if (isHostAddr(addr) || isHostName(addr)) {
		_host = addr;
	} else {
		throw std::exception();
	}
}

void ServerConfig::setPort(size_t port) {
	if (port > 65535) {
		throw std::exception();
	}
	_port = port;
}

void ServerConfig::setServerName(const std::string& name) {
	_serverNames.insert(name);
}

void ServerConfig::setErrorPages(int code, const std::string& path) {
	if (code < 300 || code > 599) {
		throw std::exception();
	}
	_errorPages[code] = path;
}

void ServerConfig::setClientMaxBodySize(uint64_t bytes) {
	_clientMaxBodySize = bytes;
}

void ServerConfig::setLocation(const std::string& path, const LocationConfig& location) {
	_locations[path] = location;
}

const std::string& ServerConfig::getHost() const {
	return _host;
}

size_t ServerConfig::getPort() const {
	return _port;
}

size_t ServerConfig::getMajorVersion() const {
	return _majorVersion;
}

size_t ServerConfig::getMinorVersion() const {
	return _minorVersion;
}

const std::set<std::string>& ServerConfig::getServerNames() const {
	return _serverNames;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const {
	return _errorPages;
}

uint64_t ServerConfig::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const std::map<std::string, LocationConfig>& ServerConfig::getLocations() const {
	return _locations;
}

bool ServerConfig::hasLocation(const std::string &location) {
	return false;
}

bool ServerConfig::isMethodAllowed(const std::string &method) {
	std::set<std::string>::const_iterator pos = _methodsAllowed.find(method);
	if (pos == _methodsAllowed.end()) {
		return false;
	}
	return true;
}

bool ServerConfig::hasName(const std::string& serverName) {
	if (_serverNames.find(serverName) != _serverNames.end()) {
		return true;
	}
	return false;
}

LocationConfig* ServerConfig::matchLocationConfig(const std::string& path) {
	if (_locations.find(path) != _locations.end()) {
		return &_locations[path];
	}
	return new LocationConfig;
}
