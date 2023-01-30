#include "../include/Request.hpp"

Request::Request() {}

Request::Request(const std::string& request) {
	size_t i = 0;

	try {
		parseRequestLine(request, &i);
		parseHeaderFields(request, &i);
		parseBody(request, &i);
		setStatus(OK);
	} catch (const std::exception& ex) {
		setStatus(400);
	}
}

Request::~Request() {}

void	Request::parseRequestLine(const std::string& request, size_t* i) {
	parseMethod(request, i);
	skipSpace(request, i);
	parseURI(request, i);
	skipSpace(request, i);
	parseVersion(request, i);
}

void Request::parseMethod(const std::string &request, size_t *i) {
	std::string method;

	method = getToken(request, i);
	setMethod(method);
}

void Request::parseURI(const std::string &request, size_t *i) {

}

void Request::parseVersion(const std::string &request, size_t *i) {

}

void Request::skipSpace(const std::string &request, size_t *i) {

}

void	Request::parseHeaderFields(const std::string& request, size_t* i) {

}

void	Request::parseBody(const std::string& request, size_t* i) {

}

void Request::setMethod(const std::string &method) {

}

void Request::setURI(const std::string &uri) {

}

void Request::setVersion(const std::string &version) {

}

void Request::setHeaderField(const std::string &fieldName, const std::string& fieldValue) {

}

void Request::setBody(const std::string &body) {

}

void Request::setStatus(size_t status) {

}

bool Request::isTchar(char a) {
	std::string tchars = "!#$%&*+-.^_|~";

	if (isalpha(a) ||
		isdigit(a) ||
		std::strchr(tchars.c_str(), a)) {
		return true;
	}
	return false;
}
