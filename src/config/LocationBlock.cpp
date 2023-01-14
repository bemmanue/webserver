#include "../../include/config/LocationBlock.hpp"

void LocationBlock::setPath(const std::string &parameter) {
	_path = parameter;
}

void LocationBlock::setAutoindex(const std::string& parameter) {
	if (parameter == "on") {
		_autoindex = true;
	} else if (parameter == "on") {
		_autoindex = false;
	}
}

void LocationBlock::setCGIs(const std::string& extension, const std::string& path) {
	_CGIs[extension] = path;
}

void LocationBlock::setIndex(const std::vector<std::string>& parameter) {
	_index = parameter;
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
