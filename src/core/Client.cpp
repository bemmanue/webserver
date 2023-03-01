#include "Client.hpp"

Client::Client(const std::vector<ServerConfig>& serverConfigs) {
	_serverConfigs = serverConfigs;
	_port = serverConfigs[0].getPort();
}

Client::~Client() {}

ServerConfig*	Client::matchServerConfig(const std::string& host) {
	for (int i = 0; i < _serverConfigs.size(); i++) {
		if (_serverConfigs[i].hasName(host)) {
			return &_serverConfigs[i];
		}
	}
	return &_serverConfigs[0];
}

std::string Client::readChunk(size_t size) {
	std::string line;

	size += 2;
	line = _request.substr(0, size);
	_request = _request.substr(size);
	return line;
}

std::string	Client::readLength(size_t size) {
	std::string line;

	line = _request.substr(0, size);
	_request = _request.substr(size);
	return line;
}

std::string Client::readLine() {
	std::string line;

	std::string::size_type pos = _request.find("\r\n");
	if (pos != std::string::npos) {
		pos += 2;
		line = _request.substr(0, pos);
		_request = _request.substr(pos, _request.size()-pos);
	} else {
		line = _request;
		_request = "";
	}
	return line;
}

void	Client::setRequest(const std::string& request) {
	_request = request;
}

void	Client::handleRequest() {
	Request		request(this);
	Response	response(&request);
	State		state;
	std::string	line;

	while (request.getState() != FORMED) {
		state = request.getState();

		if (state == PARSING_CHUNK_DATA) {
			line = readChunk(request.getExpectedBodySize());
		} else if (state == PARSING_BODY_BY_LENGTH) {
			line = readLength(request.getExpectedBodySize());
		} else {
			line = readLine();
		}
		request.parseLine(line);
	}

	std::cout << request << std::endl;
	std::cout << response.toString() << std::endl;
}

size_t Client::getPort() {
	return _port;
}
