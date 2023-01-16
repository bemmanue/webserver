#include "../../include/config/ServerBlock.hpp"

void ServerBlock::setListen(const std::vector<std::string>& value) {
//	_listen = parameter;
}

void ServerBlock::setServerName(const std::vector<std::string>& value) {
//	_server_name = parameter;
}

void ServerBlock::setErrorPages(const std::vector<std::string>& value) {
	_error_pages = value;
}

void ServerBlock::setClientMaxBodySize(const std::vector<std::string>& value) {
//	_client_max_body_size = parameter;
}

void ServerBlock::setLocation(const LocationBlock& parameter) {
	_location.push_back(parameter);
}

void ServerBlock::print() {
	std::cout << "listen: " <<  _listen << std::endl;
	std::cout << "server_name: " << _server_name << std::endl << "error_pages: ";

	for (int i = 0; i < _error_pages.size(); ++i) {
		std::cout << _error_pages[i] << " ";
	}

	std::cout << std::endl << "client_max_body_size: " << _client_max_body_size << std::endl;

	for (int i = 0; i < _location.size(); ++i) {
		std::cout << "path: " << _location[i]._path << std::endl;
		std::cout << "autoindex: " << _location[i]._autoindex << std::endl;

		std::cout << "methods_allowed: " << std::endl;
		std::cout << "CGIs: " << std::endl;
		for (int i = 0; i < _location[i]._methods_allowed.size(); ++i) {
			std::cout << _location[i]._methods_allowed[i] << " ";
		}
		std::cout << std::endl;

		std::cout << std::endl;
		std::cout << "root: " << _location[i]._root << std::endl;
	}
	std::cout << std::endl;
}