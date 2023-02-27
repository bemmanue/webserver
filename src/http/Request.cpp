#include "Request.hpp"

Request::Request(Client* client):
	_majorVersion(0),
	_minorVersion(0),
	_client(client),
	_status(OK),
	_state(PARSING_REQUEST_LINE),
	_expectedBodySize(0),
	_formed(false),
	_serverConfig(nullptr),
	_locationConfig(nullptr) {
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
	}

//	setServerConfig(_client->matchServerConfig(_host._host));
//	setLocationConfig(_serverConfig->matchLocationConfig(_requestTarget._path));

}

void	Request::parseRequestLine(const std::string& line) {
	size_t i = 0;

	// method
	setMethod(readToken(line, &i));
	if (getStatus() != OK) {
		std::cerr << "missing or invalid method" << std::endl;
		return isFormed(true);
	}

	// space
	if (!skipRequiredChar(line, &i, ' ')) {
		std::cerr << "missing space before request target" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	// request target
	setRequestTarget(readWord(line, &i));
	if (getStatus() != OK) {
		std::cerr << "invalid request target" << std::endl;
		return isFormed(true);
	}

	// space
	if (!skipRequiredChar(line, &i, ' ')) {
		std::cerr << "missing space before HTTP version" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	// version
	setVersion(readWord(line, &i));
	if (getStatus() != OK) {
		std::cerr << "invalid or unsupported HTTP version" << std::endl;
		return isFormed(true);
	}

	// CRLF
	if (!skipCRLF(line, &i)) {
		std::cerr << "missing CRLF at the end of request line" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	_state = PARSING_HEADERS;
}

void	Request::parseHeaderField(const std::string& str) {
	size_t i = 0;

	// check empty line
	if (skipCRLF(str, &i)) {
		if (!hasHeader(HOST)) {
			_status = BAD_REQUEST;
			isFormed(true);
		}
		if (!getTransferEncoding().empty() && getTransferEncoding().top() == "chunked") {
			_state = PARSING_CHUNK_SIZE;
		} else {
			_state = PARSING_BODY_BY_LENGTH;
		}
		return isFormed(false);
	}

	// header name
	std::string fieldName = readToken(str, &i);
	if (fieldName.empty()) {
		std::cerr << "missing header name" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	// required colon
	if (!skipRequiredChar(str, &i, ':')) {
		std::cerr << "missing colon before header value" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
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
		return isFormed(true);
	}

	// setting header field
	setHeader(fieldName, fieldValue);
	if (getStatus() != OK) {
		std::cerr << "invalid header field" << std::endl;
		return isFormed(true);
	}
}

void	Request::parseBody(const std::string& body) {
	setBody(body);
	isFormed(true);
}

void	Request::parseChunkedBody(const std::string& str) {
	std::string body;
	std::string chunkData;
	long 		chunkSize;
	size_t i = 0;

	chunkSize = readChunkSize(str, &i);
	if (chunkSize < 0) {
		std::cerr << "invalid or missing chunk size" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (!skipCRLF(str, &i)) {
		std::cerr << "missing CRLF after chunk size" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (chunkSize > 0) {
		chunkData = readChunkData(str, &i, chunkSize);

		if (!skipCRLF(str, &i)) {
			std::cerr << "missing CRLF after chunk data" << std::endl;
			_status = BAD_REQUEST;
			return isFormed(true);
		}

		_body.append(chunkData);

	} else {

		if (!skipCRLF(str, &i)) {
			std::cerr << "missing CRLF after chunk data2" << std::endl;
			_status = BAD_REQUEST;
			return isFormed(true);
		}

		isFormed(true);
	}
}

void	Request::setMethod(const std::string& method) {
	if (isHTTPMethod(method)) {
		_method = method;
	} else {
		_status = BAD_REQUEST;
	}
}

void	Request::setRequestTarget(const std::string& requestTarget) {
	if (isOriginForm(requestTarget)) {
		_requestTarget = requestTarget;
	} else {
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
		setMajorVersion(toDigit(version[5]));
		setMinorVersion(toDigit(version[7]));
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
	if (!host.isCorrect()) {
		_status = BAD_REQUEST;
		return;
	}

	_headers[HOST] = host;
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

size_t	Request::getStatus() const {
	return _status;
}

bool	Request::isFormed() const {
	return _formed;
}

void	Request::isFormed(bool status) {
	_formed = status;
}

ServerConfig *Request::getServerConfig() const {
	return _serverConfig;
}

LocationConfig *Request::getLocationConfig() const {
	return _locationConfig;
}

ParsingState Request::getParsingState() const {
	return _state;
}

void Request::parseChunkSize(const std::string &line) {
	size_t	i = 0;

	_expectedBodySize = readChunkSize(line, &i);
	if (_expectedBodySize < 0) {
		std::cerr << "invalid or missing chunk size" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (!skipCRLF(line, &i)) {
		std::cerr << "missing CRLF after chunk size" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (_expectedBodySize == 0) {
		_state = PARSING_TRAILER_PART;
	} else {
		_state = PARSING_CHUNK_DATA;
	}
}

void Request::parseChunkData(const std::string& line) {
	_body.append(line);
	_state = PARSING_CHUNK_SIZE;
}

void Request::parseTrailerPart(const std::string& line) {
	size_t i = 0;

	if (!skipCRLF(line, &i)) {
		_status = BAD_REQUEST;
	}
	_formed = true;
}

size_t Request::getExpectedBodySize() const {
	return _expectedBodySize;
}




