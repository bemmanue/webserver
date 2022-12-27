#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <map>

#include "ConfigException.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"

#define KW_AUTOINDEX			"autoindex"
#define KW_CGI_PASS				"cgi_pass"
#define KW_CLIENT_MAX_BODY_SIZE	"client_max_body_size"
#define KW_ERROR_PAGE			"error_page"
#define KW_INDEX				"index"
#define KW_LISTEN				"listen"
#define KW_LOCATION				"location"
#define KW_METHODS_ALLOWED		"methods_allowed"
#define KW_REDIRECT				"redirect"
#define KW_ROOT					"root"
#define KW_SERVER				"server"
#define KW_SERVER_NAME			"server_name"

#define KW_OPENING_BRACE		"{"
#define KW_CLOSING_BRACE		"}"
#define KW_SEMICOLON			";"

#define CTX_MAIN		0
#define CTX_SERVER		1
#define CTX_LOCATION	2


class Config {
public:
	void add(ServerBlock config);

private:
	std::vector<ServerBlock> config;
};

Config	parseConfigFile(const std::string& filename);


#endif //CONFIG_HPP
