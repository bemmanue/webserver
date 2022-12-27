#include "../include/LocationBlock.hpp"

void LocationBlock::setPath(const std::string &parameter) {
	_path = parameter;
}

void LocationBlock::setAutoindex(const std::vector<std::string>& parameter) {
//	_autoindex = parameter;
}

void LocationBlock::setCGIs(const std::vector<std::string> &parameter) {
	_CGIs = parameter;
}

void LocationBlock::setIndex(const std::vector<std::string>& parameter) {
//	_index = parameter;
}

void LocationBlock::setMethodsAllowed(const std::vector<std::string> &parameter) {
	_methods_allowed = parameter;
}

void LocationBlock::setRedirect(const std::vector<std::string> &parameter) {
	_redirect = parameter;
}

void LocationBlock::setRoot(const std::vector<std::string>& parameter) {
//	_root = parameter;
}
