#include "Request.hpp"

Request::Request(const ServerConfig& serverConfig, const std::string& request):
	_serverConfig(serverConfig),
	_method(""),
	_requestTarget(""),
	_query(""),
	_host(""),
	_chunked(false),
	_body(""),
	_status(OK) {
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
	_state = requestLine;

	while (i < str.size() && _status == OK) {
		switch (_state) {
			case requestLine: {
				setMethod(readToken(str, &i));
				skipRequiredChar(str, &i, ' ');
				setURI(readAbsolutePath(str, &i));
				if (str[i] == '?') {
					setQuery(readQuery(str, &++(i)));
				}
				skipRequiredChar(str, &i, ' ');

				std::string version = readVersion(str, &i);
				setMajorVersion(version[5]);
				setMinorVersion(version[7]);

				skipCRLF(str, &i);
				_state = headerField;
			}
			case headerField: {
				std::string fieldName;
				std::string fieldValue;

				while (str[i] && !isEmptyLine(str, i)) {
					fieldName = readToken(str, &i);
					skipRequiredChar(str, &i, ':');
					skipOWS(str, &i);
					fieldValue = readFieldValue(str, &i);
					skipOWS(str, &i);
					skipCRLF(str, &i);
					setHeaderField(fieldName, fieldValue);
				}
				_state = emptyLine;
			}
			case emptyLine: {
				skipCRLF(str, &i);
				_state = requestBody;
			}
			case requestBody: {
				std::string body;
				std::string chunkData;
				size_t		chunkSize;
				size_t		length;

				if (isChunked()) {
					length = 0;
					chunkSize = readChunkSize(str, &i);
					skipCRLF(str, &i);
					while (chunkSize > 0) {
						chunkData = readChunkData(str, &i, chunkSize);
						skipCRLF(str, &i);
						body.append(chunkData);
						chunkSize = readChunkSize(str, &i);
						skipCRLF(str, &i);
						length += chunkSize;
					}
					setBody(body);
				} else if (getContentLength() > 0) {
					body = str.substr(i, getContentLength());
					setBody(body);
				}

			}
		}
	}
}

//void	Request::parseRequestLine(const std::string& str, size_t* i) {
//	setMethod(readToken(str, i));
//	skipRequiredChar(str, i, ' ');
//	setURI(readAbsolutePath(str, i));
//	if (str[*i] == '?') {
//		setQuery(readQuery(str, &++(*i)));
//	}
//	skipRequiredChar(str, i, ' ');
//
//	std::string version = readVersion(str, i);
//	setMajorVersion(version[5]);
//	setMinorVersion(version[7]);
//
//	skipCRLF(str, i);
//}

//void	Request::parseHeaderFields(const std::string& str, size_t* i) {
//	std::string fieldName;
//	std::string fieldValue;
//
//	while (str[*i] && !isEmptyLine(str, *i)) {
//		fieldName = readToken(str, i);
//		skipRequiredChar(str, i, ':');
//		skipOWS(str, i);
//		fieldValue = readFieldValue(str, i);
//		skipOWS(str, i);
//		skipCRLF(str, i);
//		setHeaderField(fieldName, fieldValue);
//	}
//}

//void	Request::parseBody(const std::string& str, size_t* i) {
//	std::string body;
//	std::string chunkData;
//	size_t		chunkSize;
//	size_t		length;
//
//	if (isChunked()) {
//		length = 0;
//		chunkSize = readChunkSize(str, i);
//		skipCRLF(str, i);
//		while (chunkSize > 0) {
//			chunkData = readChunkData(str, i, chunkSize);
//			skipCRLF(str, i);
//			body.append(chunkData);
//			chunkSize = readChunkSize(str, i);
//			skipCRLF(str, i);
//			length += chunkSize;
//		}
//		setBody(body);
//	} else if (getContentLength() > 0) {
//		body = str.substr(*i, getContentLength());
//		setBody(body);
//	}
//}

void	Request::setMethod(const std::string& method) {
	if (_serverConfig.isMethodAllowed(method)) {
		_method = method;
	} else if (isHTTPMethod(method)) {
		setStatus(NOT_IMPLEMENTED);
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
