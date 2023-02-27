#include "Headers.hpp"

Headers::Headers(): _status(OK) {}

Headers::Headers(const Headers& other): _status(OK) {
	operator=(other);
}

Headers	&Headers::operator=(const Headers& other) {
	if (this != &other) {
		this->_headers = other._headers;
	}
	return *this;
}

Headers::~Headers() {}

void	Headers::setHeader(const std::string &name, const std::string &value) {
	if (name == HOST) {
		setHost(value);
	} else if (name == CONTENT_LENGTH) {
		setContentLength(value);
	} else if (name == TRANSFER_ENCODING) {
		setTransferEncoding(value);
	}
}

void	Headers::setHost(const std::string &value) {
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

void	Headers::setContentLength(const std::string &value) {
	uint64_t	length;

	if (hasHeader(CONTENT_LENGTH) && hasHeader(TRANSFER_ENCODING))	{
		_status = BAD_REQUEST;
		return;
	}
	length = strtoll(value.c_str(), nullptr, 10);
	_headers[CONTENT_LENGTH] = length;
}

void	Headers::setTransferEncoding(const std::string &value) {
	if (value != "chunked") {
		_status = NOT_IMPLEMENTED;
		return;
	}
	_headers[TRANSFER_ENCODING] = value;
}

bool	Headers::hasHeader(const std::string& name) {
	if (_headers.find(name) != _headers.end()) {
		return true;
	}
	return false;
}

size_t	Headers::getStatus() {
	return _status;
}

std::any	Headers::operator[](const std::string& headerName) {
	return _headers[headerName];
}


