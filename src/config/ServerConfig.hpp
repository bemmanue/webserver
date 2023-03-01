#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <map>
#include <set>
#include <arpa/inet.h>
#include <netdb.h>

#include "LocationConfig.hpp"

#define DEFAULT_PORT 					8080
#define DEFAULT_HOST 					"127.0.0.1"
#define DEFAULT_CLIENT_MAX_BODY_SIZE	1000000


class ServerConfig {
private:
//	unsigned short							_majorVersion;
//	unsigned short							_minorVersion;

	std::string     						_host;
	size_t            						_port;
	std::set<std::string>					_serverNames;
	std::map<int, std::string>				_errorPages;
	uint64_t 								_clientMaxBodySize;
	std::map<std::string, LocationConfig>	_locations;
	std::set<std::string>					_methodsAllowed;

public:
	ServerConfig();
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& other);
	~ServerConfig();

	void	setHost(const std::string& addr);
	void	setPort(size_t port);
	void	setServerName(const std::string& name);
	void	setErrorPages(int code, const std::string& path);
	void	setClientMaxBodySize(uint64_t bytes);
	void	setLocation(const std::string& path, const LocationConfig& location);

	bool	hasName(const std::string& serverName);
	bool	hasLocation(const std::string& path);
	bool	isMethodAllowed(const std::string& method);

	LocationConfig	*matchLocationConfig(const std::string& path);

//	size_t 											getMajorVersion() const;
//	size_t 											getMinorVersion() const;
	const std::string&								getHost() const;
	size_t											getPort() const;
	const std::set<std::string>&					getServerNames() const;
	const std::map<int, std::string>&				getErrorPages() const;
	uint64_t										getClientMaxBodySize() const;
	const std::map<std::string, LocationConfig>&	getLocations() const;


	friend std::ostream& operator<<(std::ostream& out, ServerConfig& server) {
		out << "Server:" << std::endl;
//		out << "\t" << "version: HTTP/" << server.getMajorVersion() << "." << server.getMinorVersion() << std::endl;
		out << "\t" << "host: " << server.getHost() << std::endl;
		out << "\t" << "port: " << server.getPort() << std::endl;
		std::set<std::string> names = server.getServerNames();
		for (std::set<std::string>::iterator i = names.begin(); i != names.end(); i++) {
			out << "\t" << "server_name: " << *i << std::endl;
		}
		std::map<int, std::string> ep = server.getErrorPages();
		for (std::map<int, std::string>::iterator i = ep.begin(); i != ep.end(); i++) {
			out << "\t" << "error_page: " << (*i).first << " " << (*i).second << std::endl;
		}
		std::cout << "\t" << "client_max_body_size: " << server.getClientMaxBodySize() << std::endl;
		std::map<std::string, LocationConfig> locations = server.getLocations();
		for (std::map<std::string, LocationConfig>::iterator i = locations.begin(); i != locations.end(); i++) {
			out << "\t" << "Location: " << (*i).first << std::endl;
			std::cout << (*i).second;
		}
		return out;
	}
};

#endif
