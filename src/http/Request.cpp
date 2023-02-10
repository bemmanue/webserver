#include "Request.hpp"

Request::Request():
	_method(""),
	_requestTarget(""),
	_query(""),
	_version(""),
	_host(""),
	_chunked(false),
	_body(""),
	_status(OK) {}

Request::Request(const std::string& request):
	_method(""),
	_requestTarget(""),
	_query(""),
	_version(""),
	_host(""),
	_chunked(false),
	_body(""),
	_status(OK) {
	parseRequest(request);
}

Request::~Request() {}

void	Request::parseRequest(const std::string& str) {
	size_t	i = 0;

	try {
		parseRequestLine(str, &i);
		parseHeaderFields(str, &i);
		skipCRLF(str, &i);
		parseBody(str, &i);
	} catch (const RequestException& exception) {
		setStatus(exception.getCode());
	}
	catch (const std::exception& exception) {
		setStatus(BAD_REQUEST);
	}
}

void	Request::parseRequestLine(const std::string& str, size_t* i) {
	setMethod(readToken(str, i));
	skipRequiredChar(str, i, ' ');
	setURI(readAbsolutePath(str, i));
	if (str[*i] == '?') {
		setQuery(readQuery(str, &++(*i)));
	}
	skipRequiredChar(str, i, ' ');
	setVersion(readVersion(str, i));
	skipCRLF(str, i);
}

void	Request::parseHeaderFields(const std::string& str, size_t* i) {
	std::string fieldName;
	std::string fieldValue;

	while (str[*i] && !isEmptyLine(str, *i)) {
		fieldName = readToken(str, i);
		skipRequiredChar(str, i, ':');
		skipOWS(str, i);
		fieldValue = readFieldValue(str, i);
		skipOWS(str, i);
		skipCRLF(str, i);
		setHeaderField(fieldName, fieldValue);
	}
}

void	Request::parseBody(const std::string& str, size_t* i) {
	std::string body;
	std::string chunkData;
	size_t		chunkSize;
	size_t		length;

	if (isChunked()) {
		length = 0;
		chunkSize = readChunkSize(str, i);
		skipCRLF(str, i);
		while (chunkSize > 0) {
			chunkData = readChunkData(str, i, chunkSize);
			skipCRLF(str, i);
			body.append(chunkData);
			chunkSize = readChunkSize(str, i);
			skipCRLF(str, i);
			length += chunkSize;
		}
		setBody(body);
	} else if (getContentLength() > 0) {
		body = str.substr(*i, getContentLength());
		setBody(body);
	}
}

void	Request::setMethod(const std::string& method) {
	if (global.isAllowedMethod(method)) {
		_method = method;
	} else if (global.isHTTPMethod(method)) {
		throw RequestException(NOT_IMPLEMENTED);
	} else {
		throw RequestException(BAD_REQUEST);
	}
}

void	Request::setURI(const std::string& uri) {
	_requestTarget = uri;
}

void	Request::setQuery(const std::string &query) {
	_query = query;
}

void	Request::setVersion(const std::string& version) {
	if (version.size() != 8 || version.compare(0, 5, "HTTP/") ||
		!isdigit(version[5]) || version[6] != '.' || !isdigit(version[7])) {
		throw RequestException(BAD_REQUEST);
	}
	if (!global.isSupportedVersion(version)) {
		throw RequestException(HTTP_VERSION_NOT_SUPPORTED);
	}
	_version = version;
}

void	Request::setHost(const std::string& value) {
	_host = value;
}

void	Request::setTransferEncoding(const std::string &value) {
	if (value == "chunked") {
		_chunked = true;
	} else {
		throw RequestException(NOT_IMPLEMENTED);
	}
}

void	Request::setContentLength(const std::string &value) {
	for (size_t i = 0; i < value.size(); i++) {
		if (!isdigit(value[i])) {
			throw std::exception();
		}
	}
	_length = atoi(value.c_str());
}

void	Request::setBody(const std::string &body) {
	_body = body;
}

void	Request::setStatus(size_t status) {
	_status = status;
}

void	Request::setHeaderField(const std::string &name, const std::string &value) {
	if (capitalize(name) == HOST) {
		setHost(value);
	} else if (capitalize(name) == TRANSFER_ENCODING) {
		setTransferEncoding(value);
	} else if (capitalize(name) == CONTENT_LENGTH) {
		setContentLength(value);
	}
}

std::string Request::getMethod() const {
	return _method;
}

std::string Request::getRequestTarget() const {
	return _requestTarget;
}

std::string Request::getQuery() const {
	return _query;
}

std::string Request::getVersion() const {
	return _version;
}

std::string Request::getHost() const {
	return _host;
}

size_t	Request::getContentLength() const {
	return _length;
}

std::string Request::getBody() const {
	return _body;
}

size_t	Request::getStatus() const {
	return _status;
}

bool	Request::isChunked() const {
	return _chunked;
}
