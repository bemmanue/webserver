#include "Global.hpp"

Global::Global() {
	_allowedMethods.insert(GET);
	_allowedMethods.insert(POST);
	_allowedMethods.insert(DELETE);

	_httpMethods.insert(GET);
	_httpMethods.insert(POST);
	_httpMethods.insert(PUT);
	_httpMethods.insert(HEAD);
	_httpMethods.insert(DELETE);
	_httpMethods.insert(CONNECT);
	_httpMethods.insert(OPTIONS);
	_httpMethods.insert(TRACE);
	_httpMethods.insert( PATCH);
}

Global::~Global() {}

bool Global::isAllowedMethod(const std::string& method) const {
	std::set<std::string>::const_iterator pos = _allowedMethods.find(method);
	if (pos == _allowedMethods.end()) {
		return false;
	}
	return true;
}

bool Global::isHTTPMethod(const std::string& method) const {
	std::set<std::string>::const_iterator pos = _httpMethods.find(method);
	if (pos == _httpMethods.end()) {
		return false;
	}
	return true;
}

bool Global::isServerVersion(const std::string& version) const {
	if (version == HTTP_VERSION) {
		return true;
	}
	return false;
}

bool Global::isSupportedVersion(const std::string &version) const {
	if (version[5] == HTTP_VERSION[5]) {
		return true;
	}
	return false;
}

const Global global;
