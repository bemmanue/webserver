#include "../../include/config/Config.hpp"

static std::vector<char> buffer;
static size_t i = 0;
static size_t line = 1;

const char* validMainDirectives[] =	{
		KW_SERVER, NULL
};

const char* validServerDirectives[] = {
		KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE, KW_HOST, KW_LOCATION, KW_PORT,
		KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validLocationDirectives[] =	{
		KW_AUTOINDEX, KW_CGI_PASS, KW_INDEX, KW_METHODS_ALLOWED, KW_REDIRECT,
		KW_ROOT, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validKeywords[] = {
		KW_AUTOINDEX, KW_CGI_PASS, KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE,
		KW_INDEX, KW_HOST, KW_LOCATION, KW_METHODS_ALLOWED, KW_PORT, KW_REDIRECT,
		KW_ROOT, KW_SERVER, KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE,
		KW_SEMICOLON, NULL
};


void Config::addServerBlock(const ServerBlock& c) {
	_servers.push_back(c);
}

bool Config::isEmpty() {
	return _servers.empty();
}

void Config::print() {
	for (int j = 0; j < _servers.size(); j++) {
		std::cout << "Server №" << (j + 1) << std::endl;
		_servers[j].print();
		std::cout << std::endl;
	}
}
std::vector<ServerBlock> Config::getServers() {
  return _servers;
}

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

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();

	buffer.resize(size);
	file.seekg(0, std::ios::beg);
	file.read(buffer.data(), size);
	return buffer;
}

void skipSpace() {
	for ( ; i < buffer.size(); ++i) {
		if (!isspace(buffer[i])) {
			break;
		}
		if (buffer[i] == '\n') {
			line++;
		}
	}
}

std::string	getNextToken() {
	std::string token;

	skipSpace();
	for ( ; i < buffer.size(); ++i) {
		if (isspace(buffer[i]) ||
			buffer[i] == ';' ||
			buffer[i] == '{' ||
			buffer[i] == '}') {
			if (token.empty()) {
				token.push_back(buffer[i++]);
			}
			break;
		}
		token.push_back(buffer[i]);
	}
	return token;
}

std::vector<std::string> getParameters() {
	std::vector<std::string>	value;
	std::string					token;

	while (!(token = getNextToken()).empty()) {
		if (token == KW_SEMICOLON) {
			break;
		}
		if (token == KW_OPENING_BRACE || token == KW_CLOSING_BRACE) {
			throw UnexpectedTokenConfigException(token, line);
		}

		value.push_back(token);
	}
	return value;
}

std::string	getLocationPath() {
	std::string path = getNextToken();

	if (path.empty()) {
		throw UnexpectedEndOfFileConfigException(line);
	} else if (path == KW_SEMICOLON
		|| path == KW_OPENING_BRACE
		|| path == KW_CLOSING_BRACE) {
		throw UnexpectedTokenConfigException(path, line);
	}

	return path;
}

void	setAutoindex(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_AUTOINDEX, line);
	}

	if (params[0] == "on") {
		location.setAutoindex(true);
	} else if (params[0] == "off") {
		location.setAutoindex(false);
	} else {
		throw InvalidAutoindexValueConfigException(params[0], line);
	}
}

void	setCGIs(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_CGI_PASS, line);
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
			location.setMethodsAllowed(params[j]);
		}
	} catch (const std::exception& exception) {
		throw InvalidMethodConfigException(params[j], line);
	}
}

void	setRedirect(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_REDIRECT, line);
	}

	try {
		int code = std::stoul(params[0]);
		location.setRedirect(code, params[1]);
	} catch (const std::exception& ex) {
		throw InvalidErrorPageConfigException(params[0], line);
	}
}

void	setRoot(LocationBlock& location, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_ROOT, line);
	}
	location.setRoot(params[0]);
}

void	setHost(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_HOST, line);
	}

	try {
		server.setHost(params[0]);
	} catch (const std::exception& ex) {
		throw HostNotFoundConfigException(params[0], line);
	}
}

void	setPort(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_PORT, line);
	}

	try {
		size_t port = std::stoul(params[0]);
		server.setPort(port);
	} catch (const std::exception& ex) {
		throw InvalidPortConfigException(params[0], line);
	}
}

void	setClientMaxBodySize(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_CLIENT_MAX_BODY_SIZE, line);
	}

	try {
		uint64_t size = parseSize(params[0]);
		server.setClientMaxBodySize(size);
	} catch (const std::exception& ex) {
		throw ClientMaxBodySizeConfigException(params[0], line);
	}
}

void	setErrorPages(ServerBlock& server, const std::vector<std::string>& params) {
	if (params.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_ERROR_PAGE, line);
	}

	try {
		int code = std::stoul(params[0]);
		server.setErrorPages(code, params[1]);
	} catch (const std::exception& ex) {
		throw InvalidErrorPageConfigException(params[0], line);
	}
}

void	setServerNames(ServerBlock& server, const std::vector<std::string>& params) {
	for (int j = 0; j < params.size(); j++) {
		server.setServerName(params[j]);
	}
}

LocationBlock	getLocationBlock() {
	LocationBlock				location;
	std::string					keyword;
	std::vector<std::string>	params;

	location.setPath(getLocationPath());

	if (getNextToken() != KW_OPENING_BRACE) {
		throw NoOpeningBraceConfigException(KW_LOCATION, line);
	}

	while (!(keyword = getNextToken()).empty()) {
		if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, line);
		} else if (!isValidDirective(keyword, CTX_LOCATION)) {
			throw NotAllowedDirectiveConfigException(keyword, line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		params = getParameters();
		if (keyword == KW_AUTOINDEX) {
			setAutoindex(location, params);
		} else if (keyword == KW_CGI_PASS) {
			setCGIs(location, params);
		} else if (keyword == KW_INDEX) {
			setIndex(location, params);
		} else if (keyword == KW_METHODS_ALLOWED) {
			setMethodsAllowed(location, params);
		} else if (keyword == KW_REDIRECT) {
			setRedirect(location, params);
		} else if (keyword == KW_ROOT) {
			setRoot(location, params);
		}
	}

	if (keyword.empty()) {
		throw UnexpectedEndOfFileConfigException(line);
	}

	return location;
}

ServerBlock	getServerBlock() {
	ServerBlock					server;
	LocationBlock 				location;
	std::string					keyword;
	std::vector<std::string>	params;

	if (getNextToken() != KW_OPENING_BRACE) {
		throw NoOpeningBraceConfigException(KW_SERVER, line);
	}

	while (!(keyword = getNextToken()).empty()) {
		if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, line);
		} else if (!isValidDirective(keyword, CTX_SERVER)) {
			throw NotAllowedDirectiveConfigException(keyword, line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		if (keyword == KW_LOCATION) {
			location = getLocationBlock();
			server.setLocation(location.getPath(), location);
			continue;
		}

		params = getParameters();
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
		throw UnexpectedEndOfFileConfigException(line);
	}

	return server;
}

Config	parseConfigFile(const std::string& filename) {
	Config		config;
	std::string	keyword;

	buffer = readFile(filename);

	while (!(keyword = getNextToken()).empty()) {
		if (keyword == KW_OPENING_BRACE ||
			keyword == KW_CLOSING_BRACE ||
			keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, line);
		} else if (!isValidDirective(keyword, CTX_MAIN)) {
			throw NotAllowedDirectiveConfigException(keyword, line);
		}

		if (keyword == KW_SERVER) {
			config.addServerBlock(getServerBlock());
		}
	}

	if (config.isEmpty()) {
		throw EmptyFileConfigException();
	}

	return config;
}
