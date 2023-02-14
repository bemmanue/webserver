#include "Request.hpp"

Request::Request(const std::string& request):
	_method(""),
	_requestTarget(""),
	_query(""),
	_host(""),
	_chunked(false),
	_body(""),
	_status(OK),
	_state(requestLine) {
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

	while (!isFormed()) {
		if (_state == requestLine) {
			parseRequestLine(str, &i);
		} else if (_state == headerField) {
			parseHeaderField(str, &i);
		} else if (_state == body) {
			parseBody(str, &i);
		}
	}
}

void	Request::parseRequestLine(const std::string& str, size_t* i) {
	// method
	setMethod(readToken(str, i));
	if (getStatus() != OK) {
		std::cerr << "missing or invalid method" << std::endl;
		return isFormed(true);
	}

	// space
	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "missing space before request target" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// request target
	setURI(readAbsolutePath(str, i));
	if (getStatus() != OK) {
		std::cerr << "missing requestTarget" << std::endl;
		return isFormed(true);
	}
	if (str[*i] == '?') {
		setQuery(readQuery(str, &++(*i)));
	}

	// space
	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "missing space before HTTP version" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// version
	std::string version = readVersion(str, i);
	if (version.empty()) {
		std::cerr << "missing or invalid HTTP version" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}
	setMajorVersion(toDigit(version[5]));
	setMinorVersion(toDigit(version[7]));
	if (getStatus() != OK) {
		std::cerr << "unsupported HTTP version" << std::endl;
		return isFormed(true);
	}

	// CRLF
	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	_state = headerField;
}

void	Request::parseHeaderField(const std::string& str, size_t* i) {
	// check empty line
	if (skipCRLF(str, i)) {
		_state = body;
		return isFormed(false);
	}

	// header name
	std::string fieldName = readToken(str, i);
	if (fieldName.empty()) {
		std::cerr << "missing header name" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// required colon
	if (!skipRequiredChar(str, i, ':')) {
		std::cerr << "missing colon before header value" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// optional whitespace
	skipOWS(str, i);

	// field value
	const std::string fieldValue = readFieldValue(str, i);

	// optional whitespace
	skipOWS(str, i);

	// crlf
	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF at the end of header field" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// setting header field
	setHeaderField(fieldName, fieldValue);
	if (getStatus() != OK) {
		std::cerr << "invalid header field" << std::endl;
		return isFormed(true);
	}
}

void	Request::parseBody(const std::string& str, size_t* i) {
	std::string body;
	std::string chunkData;

	if (isChunked()) {
		parseChunkedBody(str, i);
	} else if (getContentLength() > 0) {
		body = str.substr(*i, getContentLength());
		setBody(body);
		isFormed(true);
	}
}

void Request::parseChunkedBody(const std::string& str, size_t* i) {
	std::string body;
	std::string chunkData;
	long 		chunkSize;

	chunkSize = readChunkSize(str, i);
	if (chunkSize < 0) {
		std::cerr << "invalid or missing chunk size" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF after chunk size" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	if (chunkSize > 0) {
		chunkData = readChunkData(str, i, chunkSize);

		if (!skipCRLF(str, i)) {
			std::cerr << "missing CRLF after chunk data" << std::endl;
			setStatus(BAD_REQUEST);
			return isFormed(true);
		}

		_body.append(chunkData);

	} else {

		if (!skipCRLF(str, i)) {
			std::cerr << "missing CRLF after chunk data2" << std::endl;
			setStatus(BAD_REQUEST);
			return isFormed(true);
		}

		isFormed(true);
	}
}

void	Request::setMethod(const std::string& method) {
	if (isHTTPMethod(method)) {
		_method = method;
	} else {
		setStatus(BAD_REQUEST);
	}
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

void Request::isFormed(bool status) {
	_formed = status;
}

bool Request::isFormed() const {
	return _formed;
}
