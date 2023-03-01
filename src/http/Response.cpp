#include "Response.hpp"

Response::Response(Request* request):
	_majorVersion(HTTP_MAJOR_VERSION),
	_minorVersion(HTTP_MINOR_VERSION),
	_status(request->getStatus()),
	_request(request) {
	handleRequest();
}

Response::Response(const Response& other):
	Response(other._request) {
	operator=(other);
}

Response& Response::operator=(const Response &other) {
	if (this != &other) {
		_request = other._request;
	}
	return *this;
}

Response::~Response() = default;

void	Response::handleRequest() {
	if (_request->getStatus() == OK) {
		makeResponseForMethod();
	} else {
		makeResponseForError();
	}
}

void	Response::makeResponseForMethod() {
	if (_request->getMethod() == GET) {
		makeResponseForMethodGet();
	} else if (_request->getMethod() == POST) {
		makeResponseForMethodPost();
	} else if (_request->getMethod() == DELETE) {
		makeResponseForMethodDelete();
	} else {
		_status = NOT_IMPLEMENTED;
		makeResponseForError();
	}
}

void	Response::makeResponseForMethodGet() {
	std::string target = _request->getResolvedTarget();
}

void	Response::makeResponseForMethodPost() {

}

void	Response::makeResponseForMethodDelete() {

}

void	Response::makeResponseForError() {
	_reasonPhrase = getReasonPhrase(_status);
}

void	Response::makeResponseForFile() {

}

void	Response::makeResponseForDir() {

}

std::string Response::toString() {
	std::string output;
	output += "HTTP/" + std::to_string(_majorVersion) + "." + std::to_string(_minorVersion);
	output += " ";
	output += std::to_string(_status);
	output += " ";
	output += _reasonPhrase;
	output += "\r\n";
	return output;
}

