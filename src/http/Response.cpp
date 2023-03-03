#include "Response.hpp"

Response::Response(Request* request):
	_majorVersion(HTTP_MAJOR_VERSION),
	_minorVersion(HTTP_MINOR_VERSION),
	_request(request) {
	if (_request == nullptr) {
		_status = INTERNAL_SERVER_ERROR;
	} else {
		_status = _request->getStatus();
		_target = _request->getRequestTarget();
		_locationConfig = _request->getLocationConfig();
		_serverConfig = _request->getServerConfig();
	}
	_headers["Server"] = "webserver/1.0";
	handleRequest();
}

Response::Response(const Response& other):
	Response(other._request) {
	operator=(other);
}

Response& Response::operator=(const Response &other) {
	if (this != &other) {
		_request = other._request;
	}
	return *this;
}

Response::~Response() = default;

void	Response::handleRequest() {
	if (_status == OK) {
		makeResponseForMethod();
	} else {
		makeResponseForError();
	}
}

void	Response::makeResponseForMethod() {
	std::string method = _request->getMethod();

	if (method == GET) {
		makeResponseForMethodGet();
	} else if (method == POST) {
		makeResponseForMethodPost();
	} else if (method == DELETE) {
		makeResponseForMethodDelete();
	} else {
		_status = NOT_IMPLEMENTED;
		makeResponseForError();
	}
}

void	Response::makeResponseForMethodGet() {
	std::string resolvedTarget;

	if (!_locationConfig->isMethodAllowed(GET)) {
		_status = METHOD_NOT_ALLOWED;
		makeResponseForError();
		return;
	}

	resolvedTarget = _locationConfig->getRoot() + _target;
	if (!resourceExists(resolvedTarget)) {
		_status = NOT_FOUND;
		makeResponseForError();
		return;
	}

	if (isDirectory(resolvedTarget)) {
		makeResponseForDir();
	} else if (isFile(resolvedTarget)) {
		makeResponseForFile();
	} else {
		_status = FORBIDDEN;
		makeResponseForError();
	}
}

void	Response::makeResponseForDir() {
	if (setIndexFile()) {
		makeResponseForFile();
	} else if (setIndexDirectory()) {
		makeResponseForDir();
	} else {
		if (_locationConfig->getAutoindex()) {
			makeResponseForListing();
		} else {
			_status = FORBIDDEN;
			makeResponseForError();
		}
	}
}

bool	Response::setIndexFile() {
	std::vector<std::string> indices = _locationConfig->getIndices();

	for (std::size_t i = 0; i < indices.size(); ++i) {
		std::string target = _target + indices[i];
		std::string resolvedTarget = _locationConfig->getRoot() + target;

		if (isFile(resolvedTarget)) {
			_target = target;
			_locationConfig = _serverConfig->matchLocationConfig(_target);
			return true;
		}
	}
	return false;
}

bool	Response::setIndexDirectory() {
	std::vector<std::string> indices = _locationConfig->getIndices();

	for (std::size_t i = 0; i < indices.size(); ++i) {
		std::string target = _target + indices[i];
		std::string resolvedTarget = _locationConfig->getRoot() + target;

		if (isDirectory(resolvedTarget)) {
			_target = target;
			_locationConfig = _serverConfig->matchLocationConfig(_target);
			return true;
		}
	}
	return false;
}

void	Response::makeResponseForMethodPost() {
	if (!_locationConfig->isMethodAllowed(POST)) {
		_status = METHOD_NOT_ALLOWED;
		makeResponseForError();
		return;
	}

	std::string target = _locationConfig->getRoot() + _target;

	if (isDirectory(target)) {
		_status = FORBIDDEN;
		makeResponseForError();
		return;
	}

	if (isFile(target)) {
		_status = SEE_OTHER;
		makeResponseForError();
		return;
	}

	std::ofstream out(target.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (out.is_open() && out.good()) {
		out << _request->getBody();
	}

	_headers["Location"] = target;
	_status = CREATED;
}

void	Response::makeResponseForMethodDelete() {
	if (!_request->getLocationConfig()->isMethodAllowed(DELETE)) {
		_status = METHOD_NOT_ALLOWED;
		makeResponseForError();
	}

}

void	Response::makeResponseForError() {
	if (_serverConfig && _serverConfig->hasErrorPage(_status) && _target != _serverConfig->getErrorPage(_status)) {
		_target = _serverConfig->getErrorPage(_status);
		_locationConfig = _serverConfig->matchLocationConfig(_target);
		makeResponseForMethodGet();
	} else {
		_body = getPageForStatus(_status) + http_error_tail;
	}
}

void	Response::makeResponseForFile() {
	std::string resolvedTarget = _locationConfig->getRoot() + _target;

	std::ifstream file(resolvedTarget, O_RDONLY);
	if (!file.is_open()) {
		_status = INTERNAL_SERVER_ERROR;
		makeResponseForError();
		return;
	}

	std::stringstream	buffer;
	buffer << file.rdbuf();
	file.close();

	_body = buffer.str();
}

void Response::makeResponseForListing() {
	std::string resourcePath = _locationConfig->getRoot() + _target;

	static char  title[] =
			"<html>" CRLF
			"<head><title>Index of "
			;

	static char  header[] =
			"</title></head>" CRLF
			"<body>" CRLF
			"<h1>Index of "
			;

	static char  tail[] =
			"</body>" CRLF
			"</html>" CRLF
			;

//	static char  *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//							   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	_body = title + _target + header + _target + tail;

}

std::string Response::getStatusLine() {
	return getVersion() + SP + std::to_string(_status) + SP + getReasonPhrase(_status) + CRLF;
}

std::string Response::getVersion() {
	return "HTTP/" + std::to_string(_majorVersion) + "." + std::to_string(_minorVersion);
}

std::string Response::getBody() {
	return _body;
}

std::string Response::toString() {
	std::string output;

	output += getStatusLine();

	if (_request->getMinorVersion() < 1) {
		_headers[CONTENT_LENGTH] = std::to_string(_body.size());
	} else {
		_headers[TRANSFER_ENCODING] = "chunked";
	}

	for (auto i = _headers.begin(); i != _headers.end(); ++i) {
		output += (*i).first + ": " + (*i).second + CRLF;
	}
	output += CRLF;

	if (_headers.find(CONTENT_LENGTH) != _headers.end()) {
		output += getBody();
	} else {
		size_t chunkSize;
		size_t size = 1000;
		for (size_t i = 0; i <= _body.size(); i += size) {
			_body.size() - i >= size ? chunkSize = size : chunkSize = _body.size() - i;
			output += std::to_string(chunkSize) + CRLF;
			output += _body.substr(i, size) + CRLF;
		}
		output += CRLF;
	}
	return output;
}
