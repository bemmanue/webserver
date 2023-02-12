#include "Config.hpp"

static int	g_index, g_line;

const char* validMainDirectives[] =	{
		KW_SERVER, NULL
};

const char* validServerDirectives[] = {
		KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE, KW_HOST, KW_LOCATION, KW_PORT,
		KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validLocationDirectives[] =	{
		KW_AUTOINDEX, KW_CGI_PASS, KW_INDEX, KW_LIMIT_EXCEPT, KW_REDIRECT,
		KW_ROOT, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validKeywords[] = {
		KW_AUTOINDEX, KW_CGI_PASS, KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE,
		KW_INDEX, KW_HOST, KW_LOCATION, KW_LIMIT_EXCEPT, KW_PORT, KW_REDIRECT,
		KW_ROOT, KW_SERVER, KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE,
		KW_SEMICOLON, NULL
};


uint64_t	parseSize(const std::string& s) {
	uint64_t bytes;

	if (!std::isdigit(s[0])) {
		throw std::exception();
	}

	char *end = NULL;
	uint64_t res = strtoull(s.c_str(), &end, 10);

	if (end == NULL) {
		throw std::exception();
	}

	if (!strcmp(end, "b") || !strcmp(end, "B")) {
		bytes = res;
	} else if (!strcmp(end, "k") || !strcmp(end, "K")) {
		bytes = res * KB;
	} else if (!strcmp(end, "m") || !strcmp(end, "M")) {
		bytes = res * MB;
	} else if (!strcmp(end, "g") || !strcmp(end, "G")) {
		bytes = res * GB;
	} else {
		throw std::exception();
	}

	return bytes;
}

bool isValidKeyword(const std::string& keyword) {
	for (size_t j = 0; validKeywords[j]; ++j) {
		if (validKeywords[j] == keyword) {
			return true;
		}
	}
	return false;
}

bool isValidDirective(const std::string& directive, size_t context) {
	if (context == CTX_MAIN) {
		for (size_t j = 0; validMainDirectives[j]; ++j) {
			if (directive == validMainDirectives[j]) {
				return true;
			}
		}
	} else if (context == CTX_SERVER) {
		for (size_t j = 0; validServerDirectives[j]; ++j) {
			if (directive == validServerDirectives[j]) {
				return true;
			}
		}
	} else if (context == CTX_LOCATION) {
		for (size_t j = 0; validLocationDirectives[j]; ++j) {
			if (directive == validLocationDirectives[j]) {
				return true;
			}
		}
	}
	return false;
}

std::string	readFile(const std::string& filename) {
	std::ifstream		file(filename);
	std::stringstream	buffer;

	if (!file.is_open()) {
		throw ConfigException("error openning config file");
	}
	buffer << file.rdbuf();

	return buffer.str();
}

void skipSpace(const std::string& file) {
	for ( ; g_index < file.size(); ++g_index) {
		if (!isspace(file[g_index])) {
			break;
		}
		if (file[g_index] == '\n') {
			g_line++;
		}
	}
}

std::string	getNextToken(const std::string& file) {
	std::string token;

	skipSpace(file);
	for ( ; g_index < file.size(); ++g_index) {
		if (isspace(file[g_index]) ||
		file[g_index] == ';' ||
		file[g_index] == '{' ||
		file[g_index] == '}') {
			if (token.empty()) {
				token.push_back(file[g_index++]);
			}
			break;
		}
		token.push_back(file[g_index]);
	}
	return token;
}

std::vector<std::string>	getParameters(const std::string& file) {
	std::vector<std::string>	value;
	std::string					token;

	while (!(token = getNextToken(file)).empty()) {
		if (token == KW_SEMICOLON) {
			break;
		}
		if (token == KW_OPENING_BRACE || token == KW_CLOSING_BRACE) {
			throw UnexpectedTokenConfigException(token, g_line);
		}

		value.push_back(token);
	}
	return value;
}

std::string	getLocationPath(const std::string& file) {
	std::string path = getNextToken(file);

	if (path.empty()) {
		throw UnexpectedEndOfFileConfigException(g_line);
	} else if (path == KW_SEMICOLON
	|| path == KW_OPENING_BRACE
	|| path == KW_CLOSING_BRACE) {
		throw UnexpectedTokenConfigException(path, g_line);
	}

	return path;
}

void	setAutoindex(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_AUTOINDEX, g_line);
	}

	if (params[0] == "on") {
		location.setAutoindex(true);
	} else if (params[0] == "off") {
		location.setAutoindex(false);
	} else {
		throw InvalidAutoindexValueConfigException(params[0], g_line);
	}
}

void	setCGIs(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_CGI_PASS, g_line);
	}
	location.setCGIs(params[0], params[1]);
}

void	setIndex(LocationBlock& location, const std::vector<std::string>& params) {
	location.setIndex(params);
}

void	setMethodsAllowed(LocationBlock& location, const std::vector<std::string>& params) {
	int j;

	try {
		for (j = 0; j < params.size(); j++) {
			location.setLimitExcept(params[j]);
		}
	} catch (const std::exception& exception) {
		throw InvalidMethodConfigException(params[j], g_line);
	}
}

void	setRedirect(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_REDIRECT, g_line);
	}

	try {
		int code = std::stoul(params[0]);
		location.setRedirect(code, params[1]);
	} catch (const std::exception& ex) {
		throw InvalidErrorPageConfigException(params[0], g_line);
	}
}

void	setRoot(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_ROOT, g_line);
	}
	location.setRoot(params[0]);
}

void	setHost(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_HOST, g_line);
	}

	try {
		server.setHost(params[0]);
	} catch (const std::exception& ex) {
		throw HostNotFoundConfigException(params[0], g_line);
	}
}

void	setPort(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_PORT, g_line);
	}

	try {
		size_t port = std::stoul(params[0]);
		server.setPort(port);
	} catch (const std::exception& ex) {
		throw InvalidPortConfigException(params[0], g_line);
	}
}

void	setClientMaxBodySize(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_CLIENT_MAX_BODY_SIZE, g_line);
	}

	try {
		uint64_t size = parseSize(params[0]);
		server.setClientMaxBodySize(size);
	} catch (const std::exception& ex) {
		throw ClientMaxBodySizeConfigException(params[0], g_line);
	}
}

void	setErrorPages(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_ERROR_PAGE, g_line);
	}

	try {
		int code = std::stoul(params[0]);
		server.setErrorPages(code, params[1]);
	} catch (const std::exception& ex) {
		throw InvalidErrorPageConfigException(params[0], g_line);
	}
}

void	setServerNames(ServerBlock& server, const std::vector<std::string>& params) {
	for (int j = 0; j < params.size(); j++) {
		server.setServerName(params[j]);
	}
}

LocationBlock	getLocationBlock(const std::string& file) {
	LocationBlock				location;
	std::string					keyword;
	std::vector<std::string>	params;

	location.setPath(getLocationPath(file));

	if (getNextToken(file) != KW_OPENING_BRACE) {
		throw NoOpeningBraceConfigException(KW_LOCATION, g_line);
	}

	while (!(keyword = getNextToken(file)).empty()) {
		if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, g_line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, g_line);
		} else if (!isValidDirective(keyword, CTX_LOCATION)) {
			throw NotAllowedDirectiveConfigException(keyword, g_line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		params = getParameters(file);
		if (keyword == KW_AUTOINDEX) {
			setAutoindex(location, params);
		} else if (keyword == KW_CGI_PASS) {
			setCGIs(location, params);
		} else if (keyword == KW_INDEX) {
			setIndex(location, params);
		} else if (keyword == KW_LIMIT_EXCEPT) {
			setMethodsAllowed(location, params);
		} else if (keyword == KW_REDIRECT) {
			setRedirect(location, params);
		} else if (keyword == KW_ROOT) {
			setRoot(location, params);
		}
	}

	if (keyword.empty()) {
		throw UnexpectedEndOfFileConfigException(g_line);
	}

	return location;
}

ServerBlock	getServerBlock(const std::string& file) {
	ServerBlock					server;
	LocationBlock 				location;
	std::string					keyword;
	std::vector<std::string>	params;

	if (getNextToken(file) != KW_OPENING_BRACE) {
		throw NoOpeningBraceConfigException(KW_SERVER, g_line);
	}

	while (!(keyword = getNextToken(file)).empty()) {
		if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, g_line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, g_line);
		} else if (!isValidDirective(keyword, CTX_SERVER)) {
			throw NotAllowedDirectiveConfigException(keyword, g_line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		if (keyword == KW_LOCATION) {
			location = getLocationBlock(file);
			server.setLocation(location.getPath(), location);
			continue;
		}

		params = getParameters(file);
		if (keyword == KW_CLIENT_MAX_BODY_SIZE) {
			setClientMaxBodySize(server, params);
		} else if (keyword == KW_ERROR_PAGE) {
			setErrorPages(server, params);
		} else if (keyword == KW_HOST) {
			setHost(server, params);
		} else if (keyword == KW_PORT) {
			setPort(server, params);
		} else if (keyword == KW_SERVER_NAME) {
			setServerNames(server, params);
		}
	}

	if (keyword.empty()) {
		throw UnexpectedEndOfFileConfigException(g_line);
	}

	return server;
}

std::vector<ServerBlock>	parseConfigFile(const std::string& filename) throw(ConfigException) {
	std::vector<ServerBlock>	config;
	std::string					keyword;

	const std::string& file = readFile(filename);
	g_index = 0, g_line = 1;

	while (!(keyword = getNextToken(file)).empty()) {
		if (keyword == KW_OPENING_BRACE ||
		keyword == KW_CLOSING_BRACE ||
		keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, g_line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, g_line);
		} else if (!isValidDirective(keyword, CTX_MAIN)) {
			throw NotAllowedDirectiveConfigException(keyword, g_line);
		}

		if (keyword == KW_SERVER) {
			config.push_back(getServerBlock(file));
		}
	}

	if (config.empty()) {
		throw EmptyFileConfigException();
	}
	return config;
}
