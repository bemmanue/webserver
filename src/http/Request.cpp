#include "Request.hpp"

Request::Request(const std::string& request):
	_method(""),
	_requestTarget(""),
	_query(""),
	_host(""),
	_chunked(false),
	_body(""),
	_status(OK),
	_state(REQUEST_LINE) {
	parseRequest(request);
}

Request::Request(const Request &other) {
	operator=(other);
}

Request& Request::operator=(const Request &other) {
	if (this != &other) {
		_majorVersion = other._majorVersion;
		_minorVersion = other._minorVersion;
		_method = other._method;
		_requestTarget = other._requestTarget;
		_query = other._query;
		_host = other._host;
		_length = other._length;
		_chunked = other._chunked;
		_body = other._body;
		_status = other._status;
		_serverConfig = other._serverConfig;
	}
	return *this;
}

Request::~Request() {}

void	Request::parseRequest(const std::string& str) {
	size_t	i = 0;

	while (_status == OK && i < str.size()) {
		if (_state == REQUEST_LINE) {
			setStatus(parseRequestLine(str, &i));
		} else if (_state == HEADER_FIELD) {
			parseHeaderField(str, &i);
		} else if (_state == BODY) {
			parseBody(str, &i);
		}
	}
}

int	Request::parseRequestLine(const std::string& str, size_t* i) {
	std::string method = readToken(str, i);
	if (method.empty()) {
		std::cerr << "missing method" << std::endl;
		return BAD_REQUEST;
	}

	setMethod(method);
	if (!isHTTPMethod(getMethod())) {
		std::cerr << "invalid method: " << getMethod() << std::endl;
		return BAD_REQUEST;
	}

	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "absent space before request target" << std::endl;
		return BAD_REQUEST;
	}

	setURI(readAbsolutePath(str, i));
	if (getStatus() != OK) {
		std::cerr << "invalid request target" << std::endl;
		return BAD_REQUEST;
	}

	if (str[*i] == '?') {
		setQuery(readQuery(str, &++(*i)));
	}

	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "absent space before request target" << std::endl;
		return BAD_REQUEST;
	}

	std::string version = readVersion(str, i);
	if (version.empty()) {
		std::cerr << "invalid version" << std::endl;
		return BAD_REQUEST;
	}

	setMajorVersion(toDigit(version[5]));
	setMinorVersion(toDigit(version[7]));
	if (getStatus() != OK) {
		std::cerr << "invalid version" << std::endl;
		return BAD_REQUEST;
	}

	if (!skipCRLF(str, i)) {
		setStatus(BAD_REQUEST);
		return BAD_REQUEST;
	}

	_state = HEADER_FIELD;
	return BAD_REQUEST;
}

void	Request::parseHeaderField(const std::string& str, size_t* i) {
	if (isEmptyLine(str, *i)) {
		_state = BODY;
		return ;
	}

	std::string fieldName = readToken(str, i);
	if (fieldName.empty()) {
		setStatus(BAD_REQUEST);
		return ;
	}

	if (!skipRequiredChar(str, i, ':')) {
		setStatus(BAD_REQUEST);
		return ;
	}

	skipOWS(str, i);

	std::string fieldValue = readFieldValue(str, i);

	skipOWS(str, i);

	if (!skipCRLF(str, i)) {
		setStatus(BAD_REQUEST);
		return ;
	}

	setHeaderField(fieldName, fieldValue);
}

void	Request::parseBody(const std::string& str, size_t* i) {
	std::string body;
	std::string chunkData;
	size_t		chunkSize;
	size_t		length;

	if (isChunked()) {
		length = 0;
		chunkSize = readChunkSize(str, i);

		if (!skipCRLF(str, i)) {
			setStatus(BAD_REQUEST);
			return ;
		}

		while (chunkSize > 0) {
			chunkData = readChunkData(str, i, chunkSize);

			if (!skipCRLF(str, i)) {
				setStatus(BAD_REQUEST);
				return ;
			}

			body.append(chunkData);

			chunkSize = readChunkSize(str, i);

			if (!skipCRLF(str, i)) {
				setStatus(BAD_REQUEST);
				return ;
			}

			length += chunkSize;
		}
		setBody(body);

	} else if (getContentLength() > 0) {

		body = str.substr(*i, getContentLength());
		setBody(body);

	}
}

void	Request::setMethod(const std::string& method) {
//	if (isHTTPMethod(method)) {
		_method = method;
//	} else {
//		setStatus(BAD_REQUEST);
//	}
}

void	Request::setURI(const std::string& uri) {
	_requestTarget = uri;
}

void	Request::setQuery(const std::string &query) {
	_query = query;
}

void	Request::setHost(const std::string& value) {
	_host = value;
}

void	Request::setTransferEncoding(const std::string &value) {
	if (value == "chunked") {
		_chunked = true;
	} else {
		setStatus(NOT_IMPLEMENTED);
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

//std::string Request::getVersion() const {
//	return _version;
//}

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

bool Request::isSupportedVersion() const {
	return true;
}

size_t Request::getMajorVersion() const {
	return _majorVersion;
}

size_t Request::getMinorVersion() const {
	return _minorVersion;
}

void Request::setMajorVersion(unsigned short majorVersion) {
	_majorVersion = majorVersion;
}

void Request::setMinorVersion(unsigned short minorVersion) {
	_minorVersion = minorVersion;
}

void Request::setServerConfig(ServerConfig *serverConfig) {
	_serverConfig = serverConfig;
}
