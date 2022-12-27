#include "../include/Config.hpp"

static size_t line = 1;

const char* validKeywords[] = {KW_AUTOINDEX, KW_CGI_PASS, KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE,
							   KW_INDEX, KW_LISTEN, KW_LOCATION, KW_METHODS_ALLOWED, KW_REDIRECT,
							   KW_ROOT, KW_SERVER, KW_SERVER_NAME, KW_OPENING_BRACE, KW_CLOSING_BRACE,
							   KW_SEMICOLON, NULL};

const char* validMainKeywords[] = {KW_SERVER, NULL};

const char* validServerKeywords[] = {KW_CLIENT_MAX_BODY_SIZE, KW_ERROR_PAGE, KW_LISTEN, KW_LOCATION,
									 KW_SERVER_NAME, NULL};

const char* validLocationKeywords[] = {KW_AUTOINDEX, KW_CGI_PASS, KW_INDEX, KW_METHODS_ALLOWED,
									   KW_REDIRECT, KW_ROOT, NULL};

bool isValidKeyword(const std::string& keyword) {

	if (keyword.empty()) {
		return true;
	}
	for (size_t i = 0; validKeywords[i]; ++i) {
		if (validKeywords[i] == keyword) {
			return true;
		}
	}
	return false;
}

void Config::add(ServerBlock c) {
	config.push_back(c);
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
		if (buffer[*i] == '\n') {
			line++;
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

std::vector<std::string> getValue(const std::vector<char>& buffer, size_t* i) {
	std::vector<std::string>	value;
	std::string					token;

	while (*i < buffer.size()) {
		token = getNextToken(buffer, i);

		if (token == KW_SEMICOLON) {
			break;
		}
		if (token == KW_OPENING_BRACE || token == KW_CLOSING_BRACE) {
			throw ConfigException("unexpected brace", line);
		}

		value.push_back(token);
	}
	return value;
}

void	getLocationBlock(const std::vector<char>& buffer, size_t* i, ServerBlock& serverBlock) {
	LocationBlock				location;
	std::string					keyword;
	std::vector<std::string>	value;

	location.setPath(getNextToken(buffer, i));

	if (getNextToken(buffer, i) != "{") {
		throw NoOpeningBraceConfigException(KW_LOCATION, line);
	}

	while (*i < buffer.size()) {
		keyword = getNextToken(buffer, i);
//		std::cout << keyword << std::endl;

		if (keyword.empty()) {
			throw UnexpectedEndOfFileConfigException(line);
		} else if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		value = getValue(buffer, i);
		if (keyword == KW_AUTOINDEX) {
			location.setAutoindex(value);
		} else if (keyword == KW_CGI_PASS) {
			location.setCGIs(value);
		} else if (keyword == KW_INDEX) {
			location.setIndex(value);
		} else if (keyword == KW_METHODS_ALLOWED) {
			location.setMethodsAllowed(value);
		} else if (keyword == KW_REDIRECT) {
			location.setRedirect(value);
		} else if (keyword == KW_ROOT) {
			location.setRoot(value);
		}
	}
	serverBlock.setLocation(location);
}

ServerBlock	getServerBlock(const std::vector<char>& buffer, size_t* i) {
	ServerBlock					serverBlock;
	std::string					keyword;
	std::vector<std::string>	value;

	if (getNextToken(buffer, i) != KW_OPENING_BRACE) {
		throw NoOpeningBraceConfigException(KW_SERVER, line);
	}

	while (*i < buffer.size()) {
		keyword = getNextToken(buffer, i);
//		std::cout << keyword << std::endl;

		if (keyword.empty()) {
			throw UnexpectedEndOfFileConfigException(line);
		} else if (keyword == KW_OPENING_BRACE || keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		} else if (!isValidKeyword(keyword)) {
			throw UnknownDirectiveConfigException(keyword, line);
		}

		if (keyword == KW_CLOSING_BRACE) {
			break;
		}

		if (keyword == KW_LOCATION) {
			getLocationBlock(buffer, i, serverBlock);
			continue;
		}

		value = getValue(buffer, i);
		if (keyword == KW_CLIENT_MAX_BODY_SIZE) {
			serverBlock.setClientMaxBodySize(value);
		} else if (keyword == KW_ERROR_PAGE) {
			serverBlock.setErrorPages(value);
		} else if (keyword == KW_LISTEN) {
			serverBlock.setListen(value);
		} else if (keyword == KW_SERVER_NAME) {
			serverBlock.setServerName(value);
		}
	}
	return serverBlock;
}

Config	parseConfigFile(const std::string& filename) {
	Config				config;
	std::vector<char>	buffer;
	std::string			keyword;

	buffer = readFile(filename);

	size_t i = 0;
	while (i < buffer.size()) {
		keyword = getNextToken(buffer, &i);
//		std::cout << keyword << std::endl;

		if (keyword.empty()) {
			//?
		}

		if (keyword == KW_OPENING_BRACE ||
			keyword == KW_CLOSING_BRACE ||
			keyword == KW_SEMICOLON) {
			throw UnexpectedTokenConfigException(keyword, line);
		}

		if (keyword == KW_SERVER) {
			config.add(getServerBlock(buffer, &i));
		} else {
			throw UnknownDirectiveConfigException(keyword, line);
		}
	}

	return config;
}


