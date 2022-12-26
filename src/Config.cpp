#include "../include/Config.hpp"

static int line = 1;

void Config::print() {
	std::cout << "listen: " <<  _listen << std::endl;
	std::cout << "server_name: " << _server_name << std::endl << "error_pages: ";

	for (int i = 0; i < _error_pages.size(); ++i) {
		std::cout << _error_pages[i] << " ";
	}

	std::cout << std::endl << "client_max_body_size: " << _client_max_body_size << std::endl;

	for (int i = 0; i < _location.size(); ++i) {
		std::cout << "path: " << _location[i]._path << std::endl;
		std::cout << "autoindex: " << _location[i]._autoindex << std::endl;

		std::cout << "CGIs: " << std::endl;
		for (int i = 0; i < _location[i]._CGIs.size(); ++i) {
			std::cout << _location[i]._CGIs[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "index: " << _location[i]._index << std::endl;
		std::cout << "methods_allowed: " << std::endl;
		std::cout << "CGIs: " << std::endl;
		for (int i = 0; i < _location[i]._methods_allowed.size(); ++i) {
			std::cout << _location[i]._methods_allowed[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "redirect: " << std::endl;
		for (int i = 0; i < _location[i]._redirect.size(); ++i) {
			std::cout << _location[i]._redirect[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "root: " << _location[i]._root << std::endl;
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

void Config::setLocation(const Location& parameter) {
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
		if (buffer[*i] == '\n') {
			line++;
		}
		if (!isspace(buffer[*i])) {
			break;
		}
	}
}

std::string	getNextToken(const std::vector<char>& buffer, size_t* i) {
	std::string token;

	skipSpace(buffer, i);
	for ( ; *i < buffer.size(); ++*i) {
		if (isspace(buffer[*i]) ||
		buffer[*i] == ';' ||
		buffer[*i] == '{' ||
		buffer[*i] == '}') {
			if (token.empty()) {
				token.push_back(buffer[(*i)++]);
			}
			break;
		}
		token.push_back(buffer[*i]);
	}
	return token;
}

void	getListenDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expected \";\"");
	}
	config.setListen(parameter);
}

void	getClientMaxBodySizeDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expected \";\"");
	}
	config.setClientMaxBodySize(parameter);
}

void	getErrorPageDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;
	std::vector<std::string> params;

	while (*i < buffer.size()) {
		parameter = getNextToken(buffer, i);
		if (parameter == ";") {
			break;
		} else if (parameter == "{" || parameter == "}") {
			throw ConfigException("unexpected brace");
		}
		params.push_back(parameter);
	}
	config.setErrorPages(params);
}

void	getServerNameDirective(const std::vector<char>& buffer, size_t* i, Config& config) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expecting \";\"");
	}
	config.setServerName(parameter);
}

void	getAutoindexDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expected \";\"");
	}
	location.setAutoindex(parameter);
}

void	getCGIsDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;
	std::vector<std::string> params;

	while (*i < buffer.size()) {
		parameter = getNextToken(buffer, i);
		if (parameter == ";") {
			break;
		} else if (parameter == "{" || parameter == "}") {
			throw ConfigException("unexpected brace");
		}
		params.push_back(parameter);
	}
	location.setCGIs(params);
}

void	getIndexDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expected \";\"");
	}
	location.setIndex(parameter);
}

void	getMethodsAllowedDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;
	std::vector<std::string> params;

	while (*i < buffer.size()) {
		parameter = getNextToken(buffer, i);
		if (parameter == ";") {
			break;
		} else if (parameter == "{" || parameter == "}") {
			throw ConfigException("unexpected brace");
		}
		params.push_back(parameter);
	}
	location.setMethodsAllowed(params);
}

void	getRedirectDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;
	std::vector<std::string> params;

	while (*i < buffer.size()) {
		parameter = getNextToken(buffer, i);
		if (parameter == ";") {
			break;
		} else if (parameter == "{" || parameter == "}") {
			throw ConfigException("unexpected brace");
		}
		params.push_back(parameter);
	}
	location.setRedirect(params);
}

void	getRootDirective(const std::vector<char>& buffer, size_t* i, Location& location) {
	std::string parameter;

	parameter = getNextToken(buffer, i);
	if (getNextToken(buffer, i) != ";") {
		throw ConfigException("expected \";\"");
	}
	location.setRoot(parameter);
}

void	getLocationBlock(const std::vector<char>& buffer, size_t* i, Config& config) {
	Location	location;
	std::string	directiveName;

	location.setPath(getNextToken(buffer, i));

	if (getNextToken(buffer, i) != "{") {
		throw ConfigException("expecting \"{\"");
	}

	while (*i < buffer.size()) {
		directiveName = getNextToken(buffer, i);

		if (directiveName.empty()) {
			break;
		} else if (directiveName == KW_AUTOINDEX) {
			getAutoindexDirective(buffer, i, location);
		} else if (directiveName == KW_CGI_PASS) {
			getCGIsDirective(buffer, i, location);
		} else if (directiveName == KW_INDEX) {
			getIndexDirective(buffer, i, location);
		} else if (directiveName == KW_METHODS_ALLOWED) {
			getMethodsAllowedDirective(buffer, i, location);
		} else if (directiveName == KW_REDIRECT) {
			getRedirectDirective(buffer, i, location);
		} else if (directiveName == KW_ROOT) {
			getRootDirective(buffer, i, location);
		} else {
			throw UnknownDirectiveConfigException(directiveName);
		}
	}
	config.setLocation(location);
}

Config	getServerBlock(const std::vector<char>& buffer, size_t* i) {
	Config		config;
	std::string	directiveName;

	if (getNextToken(buffer, i) != "{") {
		throw ConfigException("expecting \"{\"");
	}
//	std::cout << getNextToken(buffer, i) << std::endl;

	for ( ; *i < buffer.size(); ++*i) {
		directiveName = getNextToken(buffer, i);
		std::cout << directiveName << std::endl;

		if (directiveName.empty()) {
			throw ConfigException("expecting }");
		} else if (directiveName == KW_CLIENT_MAX_BODY_SIZE) {
			getClientMaxBodySizeDirective(buffer, i, config);
		} else if (directiveName == KW_ERROR_PAGE) {
			getErrorPageDirective(buffer, i, config);
		} else if (directiveName == KW_LISTEN) {
			getListenDirective(buffer, i, config);
		} else if (directiveName == KW_LOCATION) {
			getLocationBlock(buffer, i, config);
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
	std::vector<char>	buffer;
	std::string			directiveName;

	buffer = readFile(filename);

	for (size_t i = 0; i < buffer.size(); ) {
		directiveName = getNextToken(buffer, &i);
//		std::cout << directiveName << std::endl;

		if (directiveName == KW_SERVER) {
			config.push_back(getServerBlock(buffer, &i));
		} else {
			throw UnknownDirectiveConfigException(directiveName);
		}
	}

	return config;
}
