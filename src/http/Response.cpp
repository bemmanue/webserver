#include "Response.hpp"

Response::Response(Request* request):
	_majorVersion(HTTP_MAJOR_VERSION),
	_minorVersion(HTTP_MINOR_VERSION),
	_status(OK),
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

void	Response::handleGetRequest() {

}

void	Response::makeResponseForFile() {

}

void	Response::makeResponseForDir() {

}

void	Response::handlePostRequest() {

}

void	Response::handleDeleteRequest() {

}

std::string Response::toString() {
	return _body;
}

void Response::makeResponseForError() {

}

void Response::makeResponseForMethod() {

}

