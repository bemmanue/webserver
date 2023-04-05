#include "Connection.hpp"

Connection::Connection(const std::vector<ServerConfig>& serverConfigs) {
	_serverConfigs = serverConfigs;
	_port = serverConfigs[0].getPort();
}

Connection::~Connection() {}

ServerConfig*	Connection::matchServerConfig(const std::string& host) {
	for (auto & _serverConfig : _serverConfigs) {
		if (_serverConfig.hasName(host)) {
			return &_serverConfig;
		}
	}
	return &_serverConfigs[0];
}

std::string Connection::readChunk(size_t size) {
	std::string line;

	size += 2;
	line = _str.substr(0, size);
	_str = _str.substr(size);
	return line;
}

std::string	Connection::readLength(size_t size) {
	std::string line;

	line = _str.substr(0, size);
	_str = _str.substr(size);
	return line;
}

std::string Connection::readLine() {
	std::string line;

	std::string::size_type pos = _str.find("\r\n");
	if (pos != std::string::npos) {
		pos += 2;
		line = _str.substr(0, pos);
		_str = _str.substr(pos, _str.size() - pos);
	} else {
		line = _str;
		_str = "";
	}
	return line;
}

void	Connection::setRequest(const std::string& request) {
	_str = request;
}

void	Connection::handleRequest() {
	State		state;
	std::string	line;

	_request = new Request(this);

	while (_request->getState() != FORMED) {
		state = _request->getState();

		if (state == PARSING_CHUNK_DATA) {
			line = readChunk(_request->getExpectedBodySize());
		} else if (state == PARSING_BODY_BY_LENGTH) {
			line = readLength(_request->getExpectedBodySize());
		} else {
			line = readLine();
		}
		_request->parseLine(line);
	}

	std::cout << _request << std::endl;

	_response = new Response(_request);
	std::cout << _response->getResponse() << std::endl;
}

size_t Connection::getPort() {
	return _port;
}

Connection::Connection():
	_fd(0),
	_port(0),
	_response(nullptr),
	_request(nullptr) {
}

int	Connection::readRequest() {
	int		rc;
	char	buffer[1000];

	rc = recv(_fd, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("recv() failed");
		return 0;
	}

	if (rc == 0) {
		return 0;
	}

	setRequest(buffer);
	handleRequest();
	return 1;
}

int Connection::sendResponse() {
	if (!_request) {
		std::cout << "internal server error" << std::endl;
		return 0;
	}

	std::string response = _response->getResponse();

	if ((send(_fd, response.c_str(), response.size(), 0)) < 0) {
		return 0;
	}
	return 1;
}

void Connection::setFD(int fd) {
	_fd = fd;
}

void Connection::setPort(unsigned short port) {
	_port = port;
}

void Connection::setServerConfigs(std::vector<ServerConfig> &serverConfigs) {
	_serverConfigs = serverConfigs;
}
