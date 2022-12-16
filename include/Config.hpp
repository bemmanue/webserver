#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>

#include "Location.hpp"

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


class Config {
public:
	typedef std::map<int, std::string>		ErrorPages;
	typedef std::map<std::string, Location>	Locations;

private:
	std::string		_host;
	int				_port;
	std::string		_server_name;
	ErrorPages		_error_pages;
	int				_client_max_body_size;
	Locations		_locations;
};

Config* parseConfig(const std::string& filename);


#endif //CONFIG_HPP
