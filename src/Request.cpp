#include "../include/Request.hpp"

Request::Request() {}

Request::Request(const std::string& request) {
	size_t i = 0;

	try {
		setStatus(OK);
		parseRequestLine(request, &i);
//		parseHeaderFields(request, &i);
//		parseBody(request, &i);
		std::cout << _method << std::endl;
		std::cout << _uri << std::endl;
		std::cout << _version << std::endl;
	} catch (const std::exception& ex) {
		std::cerr << "Error code: " << _status << std::endl;
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

void	Request::parseMethod(const std::string &request, size_t *i) {
	std::string method;

	method = getToken(request, *i);
	setMethod(method);
	*i += method.size();
}

void	Request::parseURI(const std::string &request, size_t *i) {
	std::string uri;
	std::string query;

	//origin form = absolute_path [? query]
	//absolute_path = 1 * ("/" segment)

	uri = getAbsolutePath(request, *i);
	setURI(uri);
	*i += uri.size();

//	if (request[*i] == '?') {
//		query = getQuery(request, *i);
//		if (!query.empty()) {
//			setQuery(query);
//			*i += query.size();
//		} else {
//			throw std::exception();
//		}
//	}
}

std::string	Request::getAbsolutePath(const std::string &request, size_t i) {
	std::string path;
	std::string segment;

	if (request[i] == '/') {
		path.push_back(request[i++]);
	} else {
		setStatus(BAD_REQUEST);
		throw std::exception();
	}

	while (segment = getSegment(request, i), !segment.empty()) {
		path.append(segment);
		i += segment.size();

		if (request[i] == '/') {
			path.push_back(request[i++]);
		} else {
			break;
		}
	}
	return path;
}

std::string	Request::getQuery(const std::string &request, size_t i) {
	std::string path;

	return path;
}

void	Request::parseVersion(const std::string &request, size_t *i) {
//	HTTP-name = %x48.54.54.50 ; HTTP
//	HTTP-version = HTTP-name "/" DIGIT "." DIGIT
	std::string version;

	if (request.compare(*i, 5, "HTTP/")) {
		setStatus(BAD_REQUEST);
		throw std::exception();
	}
	version.append("HTTP/");
	*i += 5;

	if (isdigit(request[*i]) || ) {

	}
	setVersion(version);
}

void	Request::skipSpace(const std::string &request, size_t *i) {
	if (request[*i] == ' ') {
		++*i;
	} else {
		setStatus(BAD_REQUEST);
		throw std::exception();
	}
}

std::string	Request::getToken(const std::string& request, size_t i) {
	std::string token;

	while (isTchar(request[i])) {
		token.push_back(request[i++]);
	}
	if (token.empty()) {
		throw std::exception();
	}
	return token;
}

std::string Request::getSegment(const std::string &request, size_t i) {
	std::string segment;

	while (request[i]) {
		if (isPchar(request[i])) {
			segment.push_back(request[i++]);
		} else if (isPctEncoded(request.substr(i, 3))) {
			segment.append(request.substr(i, 3));
			i += 3;
		} else {
			break;
		}
	}
	return segment;
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

void	Request::setURI(const std::string& uri) {
	_uri = uri;
}

void	Request::setVersion(const std::string& version) {
	_version = version;
}

void	Request::setHeaderField(const std::string &fieldName, const std::string& fieldValue) {

}

void	Request::setBody(const std::string &body) {

}

void	Request::setStatus(size_t status) {
	_status = status;
}

bool	Request::isTchar(char a) {
	const char *set = "!#$%&'*+-.^_`|~";

	if (isalpha(a) || isdigit(a) || std::strchr(set, a)) {
		return true;
	}
	return false;
}

bool Request::isPchar(char a) {
	if (isUnreserved(a) || isSubDelim(a) || std::strchr(":@", a)) {
		return true;
	}
	return false;
}

bool Request::isUnreserved(char a) {
	if (isalpha(a) || isdigit(a) || std::strchr("-._~", a)) {
		return true;
	}
	return false;
}

bool Request::isPctEncoded(const std::string& str) {
	if (str.size() == 3 &&
		str[0] == '%' &&
		ishexnumber(str[1]) &&
		ishexnumber(str[2])) {
		return true;
	}
	return false;
}

bool Request::isSubDelim(char a) {
	if (std::strchr("!$&'()*+,;=", a)) {
		return true;
	}
	return false;
}

void Request::setQuery(const std::string &query) {

}

