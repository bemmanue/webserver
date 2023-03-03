#include "Request.hpp"

Request::Request(Client* client):
	_majorVersion(0),
	_minorVersion(0),
	_status(OK),
	_expectedBodySize(0),
	_state(PARSING_REQUEST_LINE),
	_client(client),
	_serverConfig(nullptr),
	_locationConfig(nullptr) {
	if (_client == nullptr) {
		_status = INTERNAL_SERVER_ERROR;
		_state = FORMED;
	}
}

Request::Request(const Request &other):
	Request(nullptr) {
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
		_status = other._status;
		_expectedBodySize = other._expectedBodySize;
		_state = other._state;
		_client = other._client;
		_serverConfig = other._serverConfig;
		_locationConfig = other._locationConfig;
	}
	return *this;
}

Request::~Request() = default;

void	Request::parseLine(const std::string& line) {
	switch (_state) {
		case PARSING_REQUEST_LINE:
			parseRequestLine(line);
			break;
		case PARSING_HEADERS:
			parseHeaderField(line);
			break;
		case PARSING_BODY_BY_LENGTH:
			parseBody(line);
			break;
		case PARSING_CHUNK_SIZE:
			parseChunkSize(line);
			break;
		case PARSING_CHUNK_DATA:
			parseChunkData(line);
			break;
		case PARSING_TRAILER_PART:
			parseTrailerPart(line);
			break;
		case FORMED:
			break;
	}
}

void	Request::parseRequestLine(const std::string& line) {
	size_t i = 0;

	// method
	setMethod(readToken(line, &i));
	if (getStatus() != OK) {
		std::cerr << "missing or invalid method" << std::endl;
		_state = FORMED;
		return;
	}

	// space
	if (!skipRequiredChar(line, &i, ' ')) {
		std::cerr << "missing space before request target" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	// request target
	setRequestTarget(readWord(line, &i));
	if (getStatus() != OK) {
		std::cerr << "invalid request target" << std::endl;
		_state = FORMED;
		return;
	}

	// space
	if (!skipRequiredChar(line, &i, ' ')) {
		std::cerr << "missing space before HTTP version" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	// version
	setVersion(readWord(line, &i));
	if (getStatus() != OK) {
		std::cerr << "invalid or unsupported HTTP version" << std::endl;
		_state = FORMED;
		return;
	}

	// CRLF
	if (!skipCRLF(line, &i)) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	_state = PARSING_HEADERS;
}

void	Request::parseHeaderField(const std::string& str) {
	size_t i = 0;

	// check end of header fields
	if (skipCRLF(str, &i)) {
		checkHeaderFields();
		return;
	}

	// header name
	std::string fieldName = readToken(str, &i);
	if (fieldName.empty()) {
		std::cerr << "missing header name" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	// required colon
	if (!skipRequiredChar(str, &i, ':')) {
		std::cerr << "missing colon before header value" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	// optional whitespace
	skipOWS(str, &i);

	// field value
	const std::string fieldValue = readFieldValue(str, &i);

	// optional whitespace
	skipOWS(str, &i);

	// crlf
	if (!skipCRLF(str, &i)) {
		std::cerr << "missing CRLF at the end of header field" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	// setting header field
	setHeader(fieldName, fieldValue);
	if (getStatus() != OK) {
		std::cerr << "invalid header field" << std::endl;
		_state = FORMED;
	}
}

void Request::checkHeaderFields() {
	if (!hasHeader(HOST)) {
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	if (_method == POST && !hasHeader(CONTENT_LENGTH) && !hasHeader(TRANSFER_ENCODING)) {
		_status = LENGTH_REQUIRED;
		_state = FORMED;
		return;
	}

	matchServerConfig();
	matchLocationConfig();

	if (hasHeader(TRANSFER_ENCODING) && getTransferEncoding().top() == "chunked") {
		_state = PARSING_CHUNK_SIZE;
	} else {
		_state = PARSING_BODY_BY_LENGTH;
		if (hasHeader(CONTENT_LENGTH)) {
			_expectedBodySize = getContentLength();
		}
	}
}

void Request::parseChunkSize(const std::string &line) {
	size_t	i = 0;

	_expectedBodySize = readChunkSize(line, &i);
	if (_expectedBodySize < 0) {
		std::cerr << "invalid or missing chunk size" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	if (!skipCRLF(line, &i)) {
		std::cerr << "missing CRLF after chunk size" << std::endl;
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	if (_expectedBodySize == 0) {
		_state = PARSING_TRAILER_PART;
	} else {
		_state = PARSING_CHUNK_DATA;
	}
}

void Request::parseChunkData(const std::string& line) {
	size_t i = 0;

	_body.append(readChunkData(line, &i, _expectedBodySize));

	if (!skipCRLF(line, &i) || line[i] != '\0') {
		_status = BAD_REQUEST;
		_state = FORMED;
		return;
	}

	_state = PARSING_CHUNK_SIZE;
}

void Request::parseTrailerPart(const std::string& line) {
	size_t i = 0;

	if (!skipCRLF(line, &i)) {
		_status = BAD_REQUEST;
	}

	_state = FORMED;
}

void	Request::parseBody(const std::string& body) {
	setBody(body);
	_state = FORMED;
}

void	Request::setMethod(const std::string& method) {
	if (isHTTPMethod(method)) {
		_method = method;
	} else {
		_status = BAD_REQUEST;
	}
}

void	Request::setRequestTarget(const std::string& requestTarget) {
	_requestTarget.parseOriginForm(requestTarget);

	if (!_requestTarget.isCorrect()) {
		_status = BAD_REQUEST;
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
		setMajorVersion(version[5] - '0');
		setMinorVersion(version[7] - '0');
	} else {
		_status = BAD_REQUEST;
	}
}

void	Request::setMajorVersion(unsigned short majorVersion) {
	if (majorVersion == 1) {
		_majorVersion = majorVersion;
	} else {
		_status = HTTP_VERSION_NOT_SUPPORTED;
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

void Request::setHost(const std::string &value) {
	URI	host;

	if (hasHeader(HOST)) {
		_status = BAD_REQUEST;
		return;
	}

	host.parseHost(value);
	if (!host.isCorrect() || (host.hasPort() && host.getPort() != _client->getPort())) {
		_status = BAD_REQUEST;
		return;
	}

	_headers[HOST] = host;
}

void	Request::matchServerConfig() {
	if (_client == nullptr || !hasHeader(HOST)) {
		_status = INTERNAL_SERVER_ERROR;
		_state = FORMED;
		return;
	}
	_serverConfig = _client->matchServerConfig(getHost().getHost());
}

void	Request::matchLocationConfig() {
	if (_serverConfig == nullptr) {
		_status = INTERNAL_SERVER_ERROR;
		_state = FORMED;
		return;
	}
	_locationConfig = _serverConfig->matchLocationConfig(_requestTarget.getPath());
}

void Request::setTransferEncoding(const std::string& value) {
//	Transfer-Encoding = 1#transfer-coding
//	transfer-coding = "chunked" / "compress" / "deflate" / "gzip"
	std::stack<std::string>	transferCodings;
	std::set<std::string>	acceptedCodings{"chunked"};
	bool 					isChunked = false;
	size_t 					i = 0;

	if (hasHeader(CONTENT_LENGTH) || hasHeader(TRANSFER_ENCODING))	{
		_status = BAD_REQUEST;
		return;
	}

	while (i < value.size()) {
		if (value[i] == ',') {
			skipRequiredChar(value, &i, ',');
		}
		else if (value[i] == ' ') {
			skipOWS(value, &i);
		}
		else {
			std::string transferCoding = readToken(value, &i);

			if (transferCoding.empty()) {
				_status = BAD_REQUEST;
				return;
			}

			if (acceptedCodings.find(transferCoding) == acceptedCodings.end()){
				_status = NOT_IMPLEMENTED;
				return;
			}

			if (transferCoding == "chunked") {
				if (isChunked) {
					_status = BAD_REQUEST;
					return;
				}
				isChunked = true;
			}

			transferCodings.push(transferCoding);
		}
	}

	if (transferCodings.empty() || transferCodings.top() != "chunked") {
		_status = BAD_REQUEST;
		return;
	}

	_headers[TRANSFER_ENCODING] = transferCodings;
}

void Request::setContentLength(const std::string &value) {
	size_t	length;

	if (hasHeader(CONTENT_LENGTH) || hasHeader(TRANSFER_ENCODING))	{
		_status = BAD_REQUEST;
		return;
	}

	try {
		length = std::stoul(value);
	} catch (const std::exception& exception) {
		_status = BAD_REQUEST;
		return;
	}

	_headers[CONTENT_LENGTH] = length;
}

bool Request::hasHeader(const std::string &headerName) {
	if (_headers.find(headerName) != _headers.end()) {
		return true;
	}
	return false;
}

void	Request::setBody(const std::string &body) {
	_body = body;
}

std::string Request::getMethod() const {
	return _method;
}

std::string	Request::getRequestTarget() const {
	return _requestTarget.getPath();
}

size_t	Request::getMajorVersion() const {
	return _majorVersion;
}

size_t	Request::getMinorVersion() const {
	return _minorVersion;
}

URI	Request::getHost(){
	if (_headers.find(HOST) != _headers.end()) {
		return std::any_cast<URI>(_headers[HOST]);
	}
	return {};
}

std::stack<std::string> Request::getTransferEncoding() {
	if (_headers.find(TRANSFER_ENCODING) != _headers.end()) {
		return std::any_cast<std::stack<std::string>>(_headers[TRANSFER_ENCODING]);
	}
	return {};
}

size_t	Request::getContentLength() {
	if (_headers.find(CONTENT_LENGTH) != _headers.end()) {
		return std::any_cast<size_t>(_headers[CONTENT_LENGTH]);
	}
	return {};
}

std::string Request::getBody() const {
	return _body;
}

Status	Request::getStatus() const {
	return _status;
}

ServerConfig* Request::getServerConfig() const {
	return _serverConfig;
}

LocationConfig* Request::getLocationConfig() const {
	return _locationConfig;
}

State	Request::getState() const {
	return _state;
}

size_t	Request::getExpectedBodySize() const {
	return _expectedBodySize;
}
