#include "../../include/config/Config.hpp"

static std::vector<char> buffer;
static size_t i = 0;
static size_t line = 1;

const char* validMainDirectives[] =	{
		KW_SERVER, NULL
};

const char* validServerDirectives[] = {
		KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE, KW_LISTEN, KW_LOCATION,
		KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validLocationDirectives[] =	{
		KW_AUTOINDEX, KW_CGI_PASS, KW_INDEX, KW_METHODS_ALLOWED, KW_REDIRECT,
		KW_ROOT, KW_OPENING_BRACE, KW_CLOSING_BRACE, NULL
};

const char* validKeywords[] = {
		KW_AUTOINDEX, KW_CGI_PASS, KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE,
		KW_INDEX, KW_LISTEN, KW_LOCATION, KW_METHODS_ALLOWED, KW_REDIRECT,
		KW_ROOT, KW_SERVER, KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE,
		KW_SEMICOLON, NULL
};


void Config::add(const ServerBlock& c) {
	config.push_back(c);
}

bool Config::empty() {
	return config.empty();
}

bool isValidKeyword(const std::string& keyword) {
	for (size_t i = 0; validKeywords[i]; ++i) {
		if (validKeywords[i] == keyword) {
			return true;
		}
	}
	return false;
}

bool isValidDirective(const std::string& directive, size_t context) {
	if (context == CTX_MAIN) {
		for (size_t i = 0; validMainDirectives[i]; ++i) {
			if (directive == validMainDirectives[i]) {
				return true;
			}
		}
	} else if (context == CTX_SERVER) {
		for (size_t i = 0; validServerDirectives[i]; ++i) {
			if (directive == validServerDirectives[i]) {
				return true;
			}
		}
	} else if (context == CTX_LOCATION) {
		for (size_t i = 0; validLocationDirectives[i]; ++i) {
			if (directive == validLocationDirectives[i]) {
				return true;
			}
		}
	}
	return false;
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
	std::string path;

	path = getNextToken();

	if (path.empty()) {
		throw UnexpectedEndOfFileConfigException(line);
	} else if (path == KW_SEMICOLON
		|| path == KW_OPENING_BRACE
		|| path == KW_CLOSING_BRACE) {
		throw UnexpectedTokenConfigException(path, line);
	}

	return path;
}

void	setAutoindex(LocationBlock& location, const std::vector<std::string>& parameters) {
	if (parameters.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_AUTOINDEX, line);
	}
	location.setAutoindex(parameters[0]);
}

void	setCGIs(LocationBlock& location, const std::vector<std::string>& parameters) {
	if (parameters.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_CGI_PASS, line);
	}
	location.setCGIs(parameters[0], parameters[1]);
}

void	setMethodsAllowed(LocationBlock& location, const std::vector<std::string>& parameters) {
	int j = 0;
	try {
		for (j = 0; j < parameters.size(); j++) {
			location.setMethodsAllowed(parameters[j]);
		}
	} catch (const std::exception& exception) {
		throw InvalidMethodConfigException(parameters[j], line);
	}
}

void	setRedirect(LocationBlock& location, const std::vector<std::string>& parameters) {
	if (parameters.size() != 2) {
		throw InvalidNumberOfArgumentsConfigException(KW_REDIRECT, line);
	}
	location.setRedirect(parameters[0], parameters[1]);
}

void	setRoot(LocationBlock& location, const std::vector<std::string>& parameters) {
	if (parameters.size() != 1) {
		throw InvalidNumberOfArgumentsConfigException(KW_ROOT, line);
	}
	location.setRoot(parameters[0]);
}

LocationBlock	getLocationBlock() {
	LocationBlock				location;
	std::string					keyword;
	std::vector<std::string>	parameters;

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

		parameters = getParameters();
		if (keyword == KW_AUTOINDEX) {
			setAutoindex(location, parameters);
		} else if (keyword == KW_CGI_PASS) {
			setCGIs(location, parameters);
		} else if (keyword == KW_INDEX) {
			location.setIndex(parameters);
		} else if (keyword == KW_METHODS_ALLOWED) {
			setMethodsAllowed(location, parameters);
		} else if (keyword == KW_REDIRECT) {
			setRedirect(location, parameters);
		} else if (keyword == KW_ROOT) {
			setRoot(location, parameters);
		}
	}

	if (keyword.empty()) {
		throw UnexpectedEndOfFileConfigException(line);
	}

	return location;
}

ServerBlock	getServerBlock() {
	ServerBlock					server;
	std::string					keyword;
	std::vector<std::string>	parameters;

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
			server.setLocation(getLocationBlock());
			continue;
		}

		parameters = getParameters();
		if (keyword == KW_CLIENT_MAX_BODY_SIZE) {
			server.setClientMaxBodySize(parameters);
		} else if (keyword == KW_ERROR_PAGE) {
			server.setErrorPages(parameters);
		} else if (keyword == KW_LISTEN) {
			server.setListen(parameters);
		} else if (keyword == KW_SERVER_NAME) {
			server.setServerName(parameters);
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
			config.add(getServerBlock());
		}
	}

	if (config.empty()) {
		throw EmptyFileConfigException();
	}

	return config;
}
