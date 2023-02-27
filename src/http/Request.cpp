#include "Request.hpp"

Request::Request(Client* client, const std::string& request):
	_client(client),
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
		_status = BAD_REQUEST;
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
		_status = BAD_REQUEST;
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
		_status = BAD_REQUEST;
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
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	// required colon
	if (!skipRequiredChar(str, i, ':')) {
		std::cerr << "missing colon before header value" << std::endl;
		_status = BAD_REQUEST;
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
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (!skipCRLF(str, i)) {
		std::cerr << "missing CRLF after chunk size" << std::endl;
		_status = BAD_REQUEST;
		return isFormed(true);
	}

	if (chunkSize > 0) {
		chunkData = readChunkData(str, i, chunkSize);

		if (!skipCRLF(str, i)) {
			std::cerr << "missing CRLF after chunk data" << std::endl;
			_status = BAD_REQUEST;
			return isFormed(true);
		}

		_body.append(chunkData);

	} else {

		if (!skipCRLF(str, i)) {
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
//	Transfer-Encoding = *( "," OWS ) transfer-coding *( OWS "," [ OWS transfer-coding ] )
//	transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
//	transfer-extension = token *( OWS ";" OWS transfer-parameter )
//	transfer-parameter = token BWS "=" BWS ( token / quoted-string )
	std::stack<std::string> encodings;
	std::set<std::string>	acceptedEncodings{"chunked"};

	size_t i = 0;
	while (i < value.size()) {
		if (value[i] == ',') {
			skipRequiredChar(value, &i, ',');
		} else if (value[i] == ' ') {
			skipOWS(value, &i);
		} else {
			std::string transferCoding = readTransferCoding(value, &i);

			if (acceptedEncodings.find(transferCoding) != acceptedEncodings.end()) {
				encodings.push(transferCoding);
			} else if (transferCoding.empty()) {
				_status = BAD_REQUEST;
				return;
			} else {
				_status = NOT_IMPLEMENTED;
				return;
			}
			break;
		}
	}

	if (value != "chunked") {
		_status = NOT_IMPLEMENTED;
		return;
	}
	encodings.push(value);

	_headers[TRANSFER_ENCODING] = encodings;
}

void Request::setContentLength(const std::string &value) {
	uint64_t	length;

	if (hasHeader(CONTENT_LENGTH) && hasHeader(TRANSFER_ENCODING))	{
		_status = BAD_REQUEST;
		return;
	}
	length = strtoll(value.c_str(), nullptr, 10);
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
	return std::any_cast<URI>(_headers[HOST]);
}

std::stack<std::string> Request::getTransferEncoding() {
	return std::any_cast<std::stack<std::string>>(_headers[TRANSFER_ENCODING]);
}

size_t	Request::getContentLength() {
	return std::any_cast<size_t>(_headers[CONTENT_LENGTH]);
}

std::string Request::getBody() const {
	return _body;
}

size_t	Request::getStatus() const {
	return _status;
}

bool	Request::isChunked() const {
	return true;
}

bool	Request::isFormed() const {
	return _formed;
}

void	Request::isFormed(bool status) {
	_formed = status;
}


