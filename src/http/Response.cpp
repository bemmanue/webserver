#include "Response.hpp"

Response::Response(const ServerConfig& serverConfig, const Request& request):
	_version("HTTP/1.1"),
	_length(0),
	_serverConfig(serverConfig),
	_request(request) {
	handleRequest();
}

Response::~Response() {}

void Response::handleRequest() {
	if (_request.getStatus() != OK) {
		setStatus(_request.getStatus());
		return;
	}

	if (_request.getMethod() == GET) {
		handleGetRequest();
	} else if (_request.getMethod() == POST) {
		handlePostRequest();
	} else if (_request.getMethod() == DELETE) {
		handleDeleteRequest();
	}
}

void Response::handleGetRequest() {
	std::string requestTarget = SERVER_ROOT + _request.getRequestTarget().getAuthority();

	if (!resourceExists(requestTarget)) {
		setStatus(NOT_FOUND);
		return;
	}

	if (isFile(requestTarget)) {
		makeResponseForFile(requestTarget);
	} else if (isDirectory(requestTarget)) {
		makeResponseForDir(_request.getRequestTarget().getAuthority());
	} else {
		setStatus(FORBIDDEN);
	}
}

void	Response::makeResponseForFile(const std::string& requestTarget) {
	std::ofstream		file;
	std::stringstream	buffer;

	file.open(requestTarget.c_str(), std::ifstream::in);
	if (!file.is_open()) {
		setStatus(INTERNAL_SERVER_ERROR);
		return;
	}

	buffer << file.rdbuf();
	file.close();

	setBody(buffer.str());
	setContentLength(_body.length());
	setStatus(OK);
}

void	Response::makeResponseForDir(const std::string& requestTarget) {
	std::vector<std::string> indices;

	std::cout <<requestTarget<< std::endl;

	if (!_serverConfig.getLocations().count(requestTarget)) {
		setStatus(NOT_FOUND);
		return;
	}

	indices = _serverConfig.getLocations().at(requestTarget).getIndices();

	for (int i = 0; i < indices.size(); i++) {
		std::string path;
		path.append(SERVER_ROOT);
		path += "/" + requestTarget + "/" + indices[i];

		if (resourceExists(path)) {
			if (isFile(path)) {
				makeResponseForFile(path);
				return;
			} else if (isDirectory(path)) {
				makeResponseForDir(indices[i]);
				return;
			}
		}
	}
	setStatus(NOT_FOUND);
}

void Response::handlePostRequest() {

}

void Response::handleDeleteRequest() {

}

std::string Response::toString() {
	std::string response;

	response.append(_version);
	response.append(SP);
	response.append(statusLines[_status]);
	response.append(CRLF);

	if (_length) {
		response.append("Content-Length: ");
		response.append(std::to_string(_length));
		response.append(CRLF);
	}

	response.append(CRLF);
	response.append(_body);
	return response;
}

void Response::setStatus(size_t status) {
	_status = status;
}

void Response::setContentLength(size_t length) {
	_length = length;
}

void Response::setBody(const std::string& body) {
	_body = body;
}
