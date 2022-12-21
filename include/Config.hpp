#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
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

class ConfigException : public std::exception {
public:
	explicit ConfigException(const std::string& message): _message(message) {}
	virtual ~ConfigException() throw() {}

	virtual const char* what() const throw() {
		return _message.c_str();
	}
private:
	std::string _message;
};

class UnknownDirectiveConfigException : public std::exception {
public:
	explicit UnknownDirectiveConfigException(const std::string& name):
		_name("unknown directive \"" + name + "\"") {}
	virtual ~UnknownDirectiveConfigException() throw() {}

	virtual const char* what() const throw() {;
		return _name.c_str();
	}
private:
	std::string _name;
};

class Config {
public:
	typedef std::map<int, std::string>		ErrorPages;
	typedef std::map<std::string, Location>	Locations;

private:
	std::string					_listen;
	std::string					_server_name;
	std::vector<std::string>	_error_pages;
	std::string					_client_max_body_size;
	std::vector<Location>		_location;
//	std::string		_host;
//	int				_port;
//	std::string		_server_name;
//	ErrorPages		_error_pages;
//	int				_client_max_body_size;
//	Locations		_locations;

public:
	void setListen(const std::string& parameter);
	void setServerName(const std::string& parameter);
	void setErrorPages(const std::vector<std::string>& parameter);
	void setClientMaxBodySize(const std::string& parameter);
	void setLocation(const std::string& parameter);

	void print();
};

std::vector<Config> parseConfig(const std::string& filename);


#endif //CONFIG_HPP
