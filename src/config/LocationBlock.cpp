#include "LocationBlock.hpp"

LocationBlock::LocationBlock() {
	_methodsAllowed.insert(GET);
	_methodsAllowed.insert(POST);
	_methodsAllowed.insert(DELETE);
}

LocationBlock::LocationBlock(const LocationBlock &other) {
	operator=(other);
}

LocationBlock &LocationBlock::operator=(const LocationBlock &other) {
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

LocationBlock::~LocationBlock() {
}

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

void LocationBlock::setLimitExcept(const std::string& method) {
	std::set<std::string>::const_iterator pos = _methodsAllowed.find(method);
	if (pos == _methodsAllowed.end()) {
		throw std::exception();
	}
	_limitExcept.insert(method);
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

std::string LocationBlock::getPath() const {
	return _path;
}

bool LocationBlock::getAutoindex() const {
	return _autoindex;
}

std::map<std::string, std::string> LocationBlock::getCGIs() const {
	return _CGIs;
}

std::vector<std::string> LocationBlock::getIndices() const {
	return _index;
}

std::set<std::string> LocationBlock::getMethodsAllowed() const {
	return _limitExcept;
}

std::map<int, std::string> LocationBlock::getRedirect() const {
	return _redirect;
}

std::string LocationBlock::getRoot() const {
	return _root;
}

bool LocationBlock::isMethodAllowed(const std::string &method) {
	if (_limitExcept.empty()) {
		return true;
	}

	std::set<std::string>::const_iterator pos = _limitExcept.find(method);
	if (pos == _limitExcept.end()) {
		return false;
	}
	return true;
}
