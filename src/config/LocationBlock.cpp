#include "../../include/config/LocationBlock.hpp"

void LocationBlock::setPath(const std::string &parameter) {
	_path = parameter;
}

void LocationBlock::setAutoindex(const std::string& parameter) {
	if (parameter == "on") {
		_autoindex = true;
	} else if (parameter == "off") {
		_autoindex = false;
	}
}

void LocationBlock::setCGIs(const std::string& extension, const std::string& path) {
	_CGIs[extension] = path;
}

void LocationBlock::setIndex(const std::vector<std::string>& parameter) {
	_index = parameter;
}

void LocationBlock::setMethodsAllowed(const std::string& method) {
	if (method == "GET" ||
		method == "POST" ||
		method == "DELETE") {
		_methods_allowed.push_back(method);
	} else {
		throw std::exception();
	}
}

void LocationBlock::setRedirect(const std::string& code, const std::string& uri) {
	_redirect.code = std::stoi(code);
	_redirect.uri = uri;
}

void LocationBlock::setRoot(const std::string& path) {
	_root = path;
}
