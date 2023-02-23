#include "Request.hpp"

Request::Request(Client* client, const std::string& request):
	_client(client),
	_chunked(false),
	_contentLength(0),
	_status(OK),
	_state(requestLine) {
	parseRequest(request);
}

Request::Request(const Request &other) {
	operator=(other);
}

Request& Request::operator=(const Request &other) {
	if (this != &other) {
		_method = other._method;
		_requestTarget = other._requestTarget;
		_majorVersion = other._majorVersion;
		_minorVersion = other._minorVersion;
		_headers = other._headers;
		_body = other._body;
		_host = other._host;
		_contentLength = other._contentLength;
		_chunked = other._chunked;
		_status = other._status;
		_serverConfig = other._serverConfig;
		_locationConfig = other._locationConfig;
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

//	setServerConfig(_client->matchServerConfig(_host._host));
//	setLocationConfig(_serverConfig->matchLocationConfig(_requestTarget._path));
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
	setRequestTarget(readWord(str, i));
	if (getStatus() != OK) {
		std::cerr << "invalid request target" << std::endl;
		return isFormed(true);
	}

	// space
	if (!skipRequiredChar(str, i, ' ')) {
		std::cerr << "missing space before HTTP version" << std::endl;
		setStatus(BAD_REQUEST);
		return isFormed(true);
	}

	// version
	setVersion(readWord(str, i));
	if (getStatus() != OK) {
		std::cerr << "invalid or unsupported HTTP version" << std::endl;
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
	setHeader(fieldName, fieldValue);
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
	} else {
		isFormed(true);
	}
}

void	Request::parseChunkedBody(const std::string& str, size_t* i) {
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

void	Request::setRequestTarget(const std::string& requestTarget) {
	if (isOriginForm(requestTarget)) {
		_requestTarget = requestTarget;
	} else {
		setStatus(BAD_REQUEST);
	}
}

void	Request::setVersion(const std::string& version) {
	if (version.size() == 8 &&
		version[0] == 'H'	&&
		version[1] == 'T'	&&
		version[2] == 'T'	&&
		version[3] == 'P'	&&
		version[4] == '/'	&&
		isdigit(version[5])	&&
		version[6] == '.'	&&
		isdigit(version[7])) {
		setMajorVersion(toDigit(version[5]));
		setMajorVersion(toDigit(version[7]));
	} else {
		setStatus(BAD_REQUEST);
	}
}

void	Request::setMajorVersion(unsigned short majorVersion) {
	if (majorVersion == _serverConfig->getMajorVersion()) {
		_majorVersion = majorVersion;
	} else {
		setStatus(HTTP_VERSION_NOT_SUPPORTED);
	}
}

void	Request::setMinorVersion(unsigned short minorVersion) {
	_minorVersion = minorVersion;
}

void	Request::setHeader(const std::string& name, const std::string& value) {
	if (name == HOST) {
		setHost(value);
	} else if (name == CONTENT_LENGTH) {
		setContentLength(value);
	} else if (name == TRANSFER_ENCODING) {
		setTransferEncoding(value);
	}
}

void	Request::setHost(const std::string& value) {
//	_host.parse(value);
}

void	Request::setContentLength(const std::string& value) {
	if (_headers.find(CONTENT_LENGTH) == _headers.end() &&
		_headers.find(TRANSFER_ENCODING) == _headers.end())	{
		_headers[CONTENT_LENGTH].push_back(value);
	} else {
		setStatus(BAD_REQUEST);
	}
	_contentLength = std::stoul(value);
}

void	Request::setTransferEncoding(const std::string& value) {
	if (value == "chunked") {
		_chunked = true;
	} else {
		setStatus(NOT_IMPLEMENTED);
	}
}

void	Request::setBody(const std::string &body) {
	_body = body;
}

void	Request::setStatus(size_t status) {
	_status = status;
}

void	Request::setServerConfig(ServerConfig *serverConfig) {
	_serverConfig = serverConfig;
}

void	Request::setLocationConfig(LocationConfig *locationConfig) {
	_locationConfig = locationConfig;
}

std::string Request::getMethod() const {
	return _method;
}

std::string	Request::getRequestTarget() const {
	return _requestTarget;
}

size_t	Request::getMajorVersion() const {
	return _majorVersion;
}

size_t	Request::getMinorVersion() const {
	return _minorVersion;
}

URI Request::getHost() const {
	return _host;
}

size_t	Request::getContentLength() const {
	return _contentLength;
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

bool	Request::isFormed() const {
	return _formed;
}

void	Request::isFormed(bool status) {
	_formed = status;
}
