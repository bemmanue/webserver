#include "../../include/config/LocationBlock.hpp"

void LocationBlock::setPath(const std::string &parameter) {
	_path = parameter;
}

void LocationBlock::setAutoindex(bool status) {
	_autoindex = status;
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
		_methods_allowed.insert(method);
	} else {
		throw std::exception();
	}
}

void LocationBlock::setRedirect(int code, const std::string& uri) {
	if (code < 300 || code > 599) {
		throw std::exception();
	}
	_redirect[code] = uri;
}

void LocationBlock::setRoot(const std::string& path) {
	_root = path;
}

void LocationBlock::print() {
	std::cout << "\t\t" << "path: " << _path << std::endl;
	std::cout << "\t\t" << "autoindex: " << std::boolalpha << _autoindex << std::endl;

	for (std::map<std::string, std::string>::iterator i = _CGIs.begin(); i != _CGIs.end(); i++) {
		std::cout << "\t\t" << "cgi: " << (*i).first << " " << (*i).second << std::endl;
	}

	for (int i = 0; i < _index.size(); i++) {
		std::cout << "\t\t" << "index: " << _index[i] << std::endl;
	}

	for (std::set<std::string>::iterator i = _methods_allowed.begin(); i != _methods_allowed.end(); i++) {
		std::cout << "\t\t" << "method_allowed: " << *i << std::endl;
	}

	for (std::map<int, std::string>::iterator i = _redirect.begin(); i != _redirect.end(); i++) {
		std::cout << "\t\t" << "error_page: " << (*i).first << " " << (*i).second << std::endl;
	}

	std::cout << "\t\t" << "root: " << _root << std::endl;
}

std::string LocationBlock::getPath() const {
	return _path;
}

bool LocationBlock::getAutoindex() const {
	return _autoindex;
}

std::map<std::string, std::string> LocationBlock::getCGIs() const {
	return _CGIs;
}

std::vector<std::string> LocationBlock::getIndex() const {
	return _index;
}

std::set<std::string> LocationBlock::getMethodsAllowed() const {
	return _methods_allowed;
}

std::map<int, std::string> LocationBlock::getRedirect() const {
	return _redirect;
}

std::string LocationBlock::getRoot() const {
	return _root;
}
