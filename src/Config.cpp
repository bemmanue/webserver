#include "../include/Config.hpp"

void Config::print() {
	std::cout << "listen: " <<  _listen << std::endl;
	std::cout << "server_name: " << _server_name << std::endl << "error_pages: ";

	for (int i = 0; i < _error_pages.size(); ++i) {
		std::cout << _error_pages[i] << " ";
	}

	std::cout << std::endl << "client_max_body_size: " << _client_max_body_size << std::endl;

	for (int i = 0; i < _location.size(); ++i) {
		std::cout << _location[i] << " ";
	}
	std::cout << std::endl;
}

void Config::setListen(const std::string &parameter) {
	_listen = parameter;
}

void Config::setServerName(const std::string &parameter) {
	_server_name = parameter;
}

void Config::setErrorPages(const std::vector<std::string>& parameter) {
	_error_pages = parameter;
}

void Config::setClientMaxBodySize(const std::string &parameter) {
	_client_max_body_size = parameter;
}

void Config::setLocation(const std::string &parameter) {
	_location.push_back(parameter);
}

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	std::vector<char> buffer;

	buffer.resize(size);
	file.seekg(0, std::ios::beg);
	file.read(buffer.data(), size);
	return buffer;
}

void skipSpace(const std::vector<char>& buffer, size_t* i) {
	for ( ; *i < buffer.size(); ++*i) {
		if (!isspace(buffer[*i])) {
			break;
		}
	}
}

void skipOpeningBrace(const std::vector<char>& buffer, size_t* i) {
	for ( ; *i < buffer.size(); ++*i) {
		if (!isspace(buffer[*i])) {
			break;
		}
	}

	if (buffer[(*i)++] != '{') {
		throw ConfigException("expected \"{\"");
	}
}

std::string getDirectiveName(const std::vector<char>& buffer, size_t* i) {
	std::string name;

	skipSpace(buffer, i);
	for ( ; *i < buffer.size(); ++*i) {
		if (isspace(buffer[*i]) || buffer[*i] == ';'
			|| buffer[*i] == '{' || buffer[*i] == '}') {
			break;
		}
		name.push_back(buffer[*i]);
	}
	return name;
}

std::string getDirectiveParameter(const std::vector<char>& buffer, size_t* i) {
	std::string name;

	for ( ; *i < buffer.size(); ++*i) {
		if (isspace(buffer[*i]) ||
			buffer[*i] == ';' ||
			buffer[*i] == '{' ||
			buffer[*i] == '}') {
			break;
		}
		name.push_back(buffer[*i]);
	}
	return name;
}

void	getListenDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	skipSpace(buffer, i);
	parameter = getDirectiveParameter(buffer, i);
	skipSpace(buffer, i);
	if (buffer[*i] != ';') {
		throw std::exception();
	}
	config.setListen(parameter);
}

void	getClientMaxBodySizeDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	skipSpace(buffer, i);
	parameter = getDirectiveParameter(buffer, i);
	skipSpace(buffer, i);
	if (buffer[*i] != ';') {
		throw std::exception();
	}
	config.setClientMaxBodySize(parameter);
}

void	getErrorPageDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;
	std::vector<std::string> params;

	for ( ; *i < buffer.size(); ++*i) {
		skipSpace(buffer, i);

		parameter = getDirectiveParameter(buffer, i);
		params.push_back(parameter);

		if (buffer[*i] == ';') {
			break;
		} else if (buffer[*i] == '{' || buffer[*i] == '}') {
			throw ConfigException("invalid error page");
		}
	}
	config.setErrorPages(params);
}

void	getLocationDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	Location	location;
	std::string	directiveName;

	skipOpeningBrace(buffer, i);

	for ( ; *i < buffer.size(); ++*i) {
		directiveName = getDirectiveName(buffer, i);

		if (directiveName.empty()) {
			break;
		}

		if (directiveName == KW_AUTOINDEX) {
			getClientMaxBodySizeDirective(buffer, i, config);
		} else if (directiveName == KW_CGI_PASS) {
			getErrorPageDirective(buffer, i, config);
		} else if (directiveName == KW_INDEX) {
			getListenDirective(buffer, i, config);
		} else if (directiveName == KW_METHODS_ALLOWED) {
			getLocationDirective(buffer, i, config);
		} else if (directiveName == KW_REDIRECT) {
			;
		} else if (directiveName == KW_ROOT) {

		} else {
			throw UnknownDirectiveConfigException(directiveName);
		}
	}
}

void	getServerNameDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	skipSpace(buffer, i);
	parameter = getDirectiveParameter(buffer, i);
	skipSpace(buffer, i);
	if (buffer[*i] != ';') {
		throw ConfigException("invalid parameter");
	}
	config.setServerName(parameter);
}

Config getServerDirective(const std::vector<char>& buffer, size_t* i) {
	Config		config;
	std::string	directiveName;

	skipOpeningBrace(buffer, i);

	for ( ; *i < buffer.size(); ++*i) {
		directiveName = getDirectiveName(buffer, i);

		if (directiveName.empty()) {
			break;
		}

		if (directiveName == KW_CLIENT_MAX_BODY_SIZE) {
			getClientMaxBodySizeDirective(buffer, i, config);
		} else if (directiveName == KW_ERROR_PAGE) {
			getErrorPageDirective(buffer, i, config);
		} else if (directiveName == KW_LISTEN) {
			getListenDirective(buffer, i, config);
		} else if (directiveName == KW_LOCATION) {
			getLocationDirective(buffer, i, config);
		} else if (directiveName == KW_SERVER_NAME) {
			getServerNameDirective(buffer, i, config);
		} else {
			throw UnknownDirectiveConfigException(directiveName);
		}
	}
	return config;
}

std::vector<Config>	parseConfig(const std::string& filename) {
	std::vector<Config>	config;
	Config				newConfig;
	std::vector<char>	buffer;
	std::string			directiveName;

	buffer = readFile(filename);

	for (size_t i = 0; i < buffer.size(); ++i) {
		directiveName = getDirectiveName(buffer, &i);

		if (directiveName == KW_SERVER) {
			newConfig = getServerDirective(buffer, &i);
			config.push_back(newConfig);
		} else {
			throw UnknownDirectiveConfigException(directiveName);
		}
	}

	return config;
}
