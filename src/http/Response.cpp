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

void	Response::Post() {
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

void	Response::Delete() {
	if (!_request->getLocationConfig()->isMethodAllowed(DELETE)) {
		_status = METHOD_NOT_ALLOWED;
		makeResponseForError();
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

	_body = title + _target + header + _target;
	_body += "</h1><hr><pre><a href=\"../\">../</a>" CRLF;
	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		_body += "<a href=\"" + (std::string)entry.path().filename() + "\">";
		std::string show = entry.path().filename();
		if (show.size() > 50) {
			show = show.substr(0, 47) + "..&gt;";
		}
		_body += show;
		_body += "</a>";
		std::string empty(51, ' ');
		if (show.size() <= 50) {
			_body += empty.substr(0, 51 - show.size());
		} else {
			_body += empty.substr(0, 1);
		}
		_body += timeToString(last_write_time(entry.path()));
		std::string size;
		if (entry.is_directory()) {
			size = "-";
		} else {
			size = std::to_string(entry.file_size());
		}
		empty = "                    ";
		_body += empty.substr(0, 20 - size.size());
		_body += size;
		_body += CRLF;
	}
	_body += "</pre><hr>";
	_body += tail;
}

template <typename TP>
std::time_t to_time_t(TP tp) {
	auto time = tp - TP::clock::now() + std::chrono::system_clock::now();
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(time);

	return std::chrono::system_clock::to_time_t(sctp);
}

std::string Response::timeToString(std::filesystem::file_time_type point) {
	std::time_t			tt = to_time_t(point);
	std::tm				*gmt = std::gmtime(&tt);
	std::stringstream	buffer;

	buffer << std::put_time(gmt, "%d-%b-%Y %H:%M");
	return buffer.str();
}

void Response::setStatus(Status status) {
	_status = status;
}

Status Response::getStatus() {
	return _status;
}

