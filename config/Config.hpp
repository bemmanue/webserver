#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "ConfigException.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"

#define KW_AUTOINDEX			"autoindex"
#define KW_CGI_PASS				"cgi_pass"
#define KW_CLIENT_MAX_BODY_SIZE	"client_max_body_size"
#define KW_ERROR_PAGE			"error_page"
#define KW_INDEX				"index"
#define KW_HOST					"host"
#define KW_LOCATION				"location"
#define KW_METHODS_ALLOWED		"methods_allowed"
#define KW_PORT					"port"
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

#define KB	1000
#define MB	1000 * KB
#define GB	1000 * MB


class Config {
private:
	std::vector<ServerBlock>	_servers;

public:
	Config();
	Config(const Config& other);
	Config& operator=(const Config& other);
	~Config();

	void	setServerBlock(const ServerBlock& c);
	bool	isEmpty();
	void	print(); //debug

    std::vector<ServerBlock>	getServerBlocks();
};

std::string					readFile(const std::string& filename);
std::string					getNextToken();
ServerBlock					getServerBlock();
LocationBlock				getLocationBlock();
std::vector<std::string>	getParameters();
std::string					getLocationPath();
void						skipSpace();

Config		parseConfigFile(const std::string& filename) throw(ConfigException);
void		setServerNames(ServerBlock& server, const std::vector<std::string>& params);
void		setErrorPages(ServerBlock& server, const std::vector<std::string>& params);
void		setClientMaxBodySize(ServerBlock& server, const std::vector<std::string>& params);
void		setPort(ServerBlock& server, const std::vector<std::string>& params);
void		setHost(ServerBlock& server, const std::vector<std::string>& params);
void		setRoot(LocationBlock& location, const std::vector<std::string>& params);
void		setRedirect(LocationBlock& location, const std::vector<std::string>& params);
void		setMethodsAllowed(LocationBlock& location, const std::vector<std::string>& params);
void		setIndex(LocationBlock& location, const std::vector<std::string>& params);
void		setCGIs(LocationBlock& location, const std::vector<std::string>& params);
void		setAutoindex(LocationBlock& location, const std::vector<std::string>& params);

bool		isValidDirective(const std::string& directive, size_t context);
bool		isValidKeyword(const std::string& keyword);
uint64_t	parseSize(const std::string& s);





#endif //CONFIG_HPP
