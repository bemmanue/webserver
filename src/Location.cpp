#include "../include/Location.hpp"

void Location::setPath(const std::string &parameter) {
	_path = parameter;
}

void Location::setAutoindex(const std::string &parameter) {
	_autoindex = parameter;
}

void Location::setCGIs(const std::string &parameter) {
	_CGIs = parameter;
}

void Location::setIndex(const std::string &parameter) {
	_index = parameter;
}

void Location::setMethodsAllowed(const std::string &parameter) {
	_methods_allowed = parameter;
}

void Location::setRedirect(const std::string &parameter) {
	_redirect = parameter;
}

void Location::setRoot(const std::string &parameter) {
	_root = parameter;
}
