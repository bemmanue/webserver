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
	if (getStatus() < BAD_REQUEST) {
		handleMethod();
	}

	if (getStatus() >= BAD_REQUEST) {
		makeResponseForError();
	}
}

void	Response::handleMethod() {
	std::string method = _request->getMethod();

	if (method == GET) {
		Get();
	} else if (method == POST) {
		Post();
	} else if (method == DELETE) {
		Delete();
	} else {
		setStatus(NOT_IMPLEMENTED);
	}
}

void	Response::Get() {
	std::string resolvedTarget;

	if (!_locationConfig->isMethodAllowed(GET)) {
		setStatus(METHOD_NOT_ALLOWED);
		return;
	}

	resolvedTarget = _locationConfig->getRoot() + _target;
	if (!resourceExists(resolvedTarget)) {
		setStatus(NOT_FOUND);
		return;
	}

	if (isDirectory(resolvedTarget)) {
		makeResponseForDir();
	} else if (isFile(resolvedTarget)) {
		makeResponseForFile();
	} else {
		setStatus(NOT_FOUND);
	}
}

void	Response::Post() {
	if (!_locationConfig->isMethodAllowed(POST)) {
		setStatus(METHOD_NOT_ALLOWED);
		return;
	}

	std::string target = _locationConfig->getRoot() + _target;

	if (isDirectory(target)) {
		const std::string& filename = "POST_" + std::to_string(std::rand());
    	const std::string& fullpath = target + "/" + filename;
		std::ofstream outfile(fullpath);
		outfile << _request->getBody() << std::endl;
		outfile.close();
		setStatus(CREATED);
	} else {
		setStatus(BAD_REQUEST);
	}
}

void	Response::Delete() {
	if (!_request->getLocationConfig()->isMethodAllowed(DELETE)) {
		setStatus(METHOD_NOT_ALLOWED);
		return;
	}

	std::string target = _locationConfig->getRoot() + _target;

	if (isFile(target)) {
		std::remove(target);
		setStatus(OK);
	} else {
		setStatus(BAD_REQUEST);
	}
}

void	Response::makeResponseForFile() {
	std::string			resolvedTarget;
	std::ifstream		file;
	std::stringstream	buffer;

	resolvedTarget = _locationConfig->getRoot() + _target;

	file.open(resolvedTarget, O_RDONLY);
	if (!file.is_open()) {
		setStatus(INTERNAL_SERVER_ERROR);
		return;
	}

	buffer << file.rdbuf();
	file.close();

	setBody(buffer.str());
}

void	Response::makeResponseForDir() {
	if (setIndexFile()) {
		makeResponseForFile();
	} else if (setIndexDirectory()) {
		makeResponseForDir();
	} else if (_locationConfig->getAutoindex()) {
		makeResponseForListing();
	} else {
		setStatus(NOT_FOUND);
	}
}

void	Response::makeResponseForError() {
	if (_serverConfig && _serverConfig->hasErrorPage(_status) && _target != _serverConfig->getErrorPage(_status)) {
		_target = _serverConfig->getErrorPage(_status);
		_locationConfig = _serverConfig->matchLocationConfig(_target);
		Get();
	} else {
		_body = getPageForStatus(_status) + http_error_tail;
	}
}

bool	Response::setIndexFile() {
	std::vector<std::string>	indices;
	std::string					target;
	std::string					resolvedTarget;

	indices = _locationConfig->getIndices();
	for (auto & index : indices) {
		target = _target + index;
		resolvedTarget = _locationConfig->getRoot() + target;

		if (isFile(resolvedTarget)) {
			_target = target;
			_locationConfig = _serverConfig->matchLocationConfig(_target);
			return true;
		}
	}
	return false;
}

bool	Response::setIndexDirectory() {
	std::vector<std::string>	indices;
	std::string					target;
	std::string					resolvedTarget;

	indices = _locationConfig->getIndices();
	for (auto & index : indices) {
		target = _target + index;
		resolvedTarget = _locationConfig->getRoot() + target;

		if (isDirectory(resolvedTarget)) {
			_target = target;
			_locationConfig = _serverConfig->matchLocationConfig(_target);
			return true;
		}
	}
	return false;
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

std::string Response::getResponse() {
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
		for (size_t i = 0; i < _body.size(); i += size) {
			_body.size() - i >= size ? chunkSize = size : chunkSize = _body.size() - i;
			output += toHexString((int)chunkSize) + CRLF;
			output += _body.substr(i, size) + CRLF;
		}
		output += "0" CRLF;
		output += CRLF;
	}
	return output;
}

void Response::makeResponseForListing() {
	std::string path = _locationConfig->getRoot() + _target;
	std::string body;

	body =	HTML_BEG CRLF
			HEAD_BEG TITLE_BEG "Index of " + path + TITLE_END HEAD_END
			BODY_BEG CRLF
			H1_BEG "Index of " + path + H1_END CRLF
			HR_BEG PRE_BEG CRLF
			HREF_BEG + "../" + HREF_END "../" A_END CRLF;

	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		std::string ref = (std::string)entry.path().filename();

		std::string name = ref;
		if (name.size() > 50) {
			name = name.substr(0, 47) + "..&gt;";
		}

		std::string space(51 - name.substr(0, 50).size(), ' ');

		std::string time = timeToString(last_write_time(entry.path()));

		std::string size;
		if (entry.is_directory()) {
			size = "-";
		} else {
			size = std::to_string(entry.file_size());
		}

		std::string space2(20 - size.size(), ' ');

		body += HREF_BEG + ref + HREF_END + name + A_END + space + time + space2 + size + CRLF;

	}

	body +=	PRE_END HR_END CRLF
			BODY_END CRLF
			HTML_END CRLF;

	setBody(body);
}



void Response::setStatus(Status status) {
	_status = status;
}

void Response::setBody(const std::string &body) {
	_body = body;
}

Status Response::getStatus() {
	return _status;
}
