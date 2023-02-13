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
	// method
	const std::string method = readToken(str, i);
	if (method.empty()) {
		std::cerr << "missing method" << std::endl;
		return BAD_REQUEST;
	}

	if (!isHTTPMethod(method)) {
		std::cerr << "invalid method: " << getMethod() << std::endl;
		return BAD_REQUEST;
	}

	setMethod(method);


	// space
	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "missing space before request target" << std::endl;
		return BAD_REQUEST;
	}


	// request target
	const std::string absolutePath = readAbsolutePath(str, i);
	if (absolutePath.empty()) {
		std::cerr << "missing requestTarget" << std::endl;
		return BAD_REQUEST;
	}

	setURI(absolutePath);

	if (str[*i] == '?') {
		setQuery(readQuery(str, &++(*i)));
	}


	// space
	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "missing space before HTTP version" << std::endl;
		return BAD_REQUEST;
	}


	// version
	std::string version = readVersion(str, i);
	if (version.empty()) {
		std::cerr << "missing or invalid version" << std::endl;
		return BAD_REQUEST;
	}

	if (toDigit(version[5]) != 1) {
		std::cerr << "invalid version" << std::endl;
		return BAD_REQUEST;
	}

	setMajorVersion(toDigit(version[5]));
	setMinorVersion(toDigit(version[7]));


	// CRLF
	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		return BAD_REQUEST;
	}

	_state = HEADER_FIELD;

	return OK;
}

int	Request::parseHeaderField(const std::string& str, size_t* i) {
	if (isEmptyLine(str, *i)) {
		_state = BODY;
		return OK;
	}

	// header name
	std::string fieldName = readToken(str, i);
	if (fieldName.empty()) {
		std::cerr << "missing header name" << std::endl;
		return BAD_REQUEST;
	}


	// required colon
	if (!skipRequiredChar(str, i, ':')) {
		std::cerr << "missing colon before header value" << std::endl;
		return BAD_REQUEST;
	}

	skipOWS(str, i);

	const std::string fieldValue = readFieldValue(str, i);

	skipOWS(str, i);

	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		return BAD_REQUEST;
	}

	setHeaderField(fieldName, fieldValue);
	if (getStatus() != OK) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		return BAD_REQUEST;
	}

	return OK;
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
	_method = method;
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
