#include "LocationConfig.hpp"

LocationConfig::LocationConfig():
	_autoindex(false) {
	_methodsAllowed.insert(GET);
	_methodsAllowed.insert(POST);
	_methodsAllowed.insert(DELETE);
}

LocationConfig::LocationConfig(const LocationConfig &other):
	LocationConfig() {
	operator=(other);
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other) {
	if (this != &other) {
		_path = other._path;
		_autoindex = other._autoindex;
		_CGIs = other._CGIs;
		_index = other._index;
		_limitExcept = other._limitExcept;
		_redirect = other._redirect;
		_root = other._root;
	}
	return *this;
}

LocationConfig::~LocationConfig() = default;

void LocationConfig::setPath(const std::string& parameter) {
	if (parameter.empty() || parameter.front() != '/' || parameter.back() != '/') {
		throw std::exception();
	}
	_path = URI::normalize(URI::removeDotSegments(parameter));
}

void LocationConfig::setAutoindex(bool status) {
	_autoindex = status;
}

void LocationConfig::setCGIs(const std::string& extension, const std::string& path) {
	_CGIs[extension] = path;
}

void LocationConfig::setIndex(const std::vector<std::string>& parameter) {
	_index = parameter;
}

void LocationConfig::setLimitExcept(const std::string& method) {
	std::set<std::string>::const_iterator pos = _methodsAllowed.find(method);
	if (pos == _methodsAllowed.end()) {
		throw std::exception();
	}
	_limitExcept.insert(method);
}

void LocationConfig::setRedirect(int code, const std::string& uri) {
	if (code < 300 || code > 599) {
		throw std::exception();
	}
	_redirect[code] = uri;
}

void LocationConfig::setRoot(const std::string& path) {
	_root = path;
}

std::string LocationConfig::getPath() const {
	return _path;
}

bool LocationConfig::getAutoindex() const {
	return _autoindex;
}

std::map<std::string, std::string> LocationConfig::getCGIs() const {
	return _CGIs;
}

std::vector<std::string> LocationConfig::getIndices() const {
	return _index;
}

std::set<std::string> LocationConfig::getMethodsAllowed() const {
	return _limitExcept;
}

std::map<int, std::string> LocationConfig::getRedirect() const {
	return _redirect;
}

std::string LocationConfig::getRoot() const {
	return _root;
}

bool LocationConfig::isMethodAllowed(const std::string &method) {
	if (_limitExcept.empty()) {
		return true;
	}

	std::set<std::string>::const_iterator pos = _limitExcept.find(method);
	if (pos == _limitExcept.end()) {
		return false;
	}
	return true;
}
