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
		;
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

std::string	Request::getToken(const std::string& request, size_t* i) {
	std::string token;

	while (isTchar(request[*i])) {
		token.push_back(request[(*i)++]);
	}
	return token;
}


void	Request::parseMethod(const std::string &request, size_t *i) {
	std::string method;

	method = getToken(request, i);
	setMethod(method);
}

void	Request::parseURI(const std::string &request, size_t *i) {
	std::string uri;

	uri = getToken(request, i);
	setURI(uri);
}

void	Request::parseVersion(const std::string &request, size_t *i) {

}

void	Request::skipSpace(const std::string &request, size_t *i) {
	if (request[*i] == ' ') {
		++*i;
	} else {
		setStatus(BAD_REQUEST);
		throw std::exception;
	}
}

void	Request::parseHeaderFields(const std::string& request, size_t* i) {

}

void	Request::parseBody(const std::string& request, size_t* i) {

}

void	Request::setMethod(const std::string &method) {
	if (method == "GET" ||
		method == "POST" ||
		method == "DELETE") {
		_method = method;
	} else if (method == "PUT" ||
		method == "OPTIONS" ||
		method == "CONNECT" ||
		method == "PATCH" ||
		method == "TRACE" ||
		method == "HEAD") {
		setStatus(NOT_IMPLEMENTED);
		throw std::exception();
	} else {
		setStatus(BAD_REQUEST);
		throw std::exception();
	}
}

void	Request::setURI(const std::string &uri) {

}

void	Request::setVersion(const std::string &version) {

}

void	Request::setHeaderField(const std::string &fieldName, const std::string& fieldValue) {

}

void	Request::setBody(const std::string &body) {

}

void	Request::setStatus(size_t status) {

}

bool	Request::isTchar(char a) {
	const char *tchars = "!#$%&'*+-.^_`|~";

	if (isalpha(a) ||
		isdigit(a) ||
		std::strchr(tchars, a)) {
		return true;
	}
	return false;
}
