#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <map>
#include <set>
#include <arpa/inet.h>
#include <netdb.h>

#include "LocationBlock.hpp"
#include "Global.hpp"

#define DEFAULT_PORT 					8080
#define DEFAULT_HOST 					"127.0.0.1"
#define DEFAULT_CLIENT_MAX_BODY_SIZE	1000000


class ServerBlock {
private:
	size_t									_majorVersion;
	size_t									_minorVersion;

	std::string     						_host;
	size_t            						_port;
	std::set<std::string>					_server_names;
	std::map<int, std::string>				_error_pages;
	uint64_t 								_client_max_body_size;
	std::map<std::string, LocationBlock>	_locations;
	std::set<std::string>					_methods_allowed;

public:
	ServerBlock();
	ServerBlock(const ServerBlock& other);
	ServerBlock& operator=(const ServerBlock& other);
	~ServerBlock();

	void	setHost(const std::string& addr);
	void	setPort(size_t port);
	void	setServerName(const std::string& name);
	void	setErrorPages(int code, const std::string& path);
	void	setClientMaxBodySize(uint64_t bytes);
	void	setLocation(const std::string& path, const LocationBlock& location);

	bool	hasLocation(const std::string& location);
	bool	isMethodAllowed(const std::string& method);

	size_t 										getMajorVersion() const;
	size_t 										getMinorVersion() const;
	const std::string&							getHost() const;
	size_t										getPort() const;
	const std::set<std::string>&				getServerNames() const;
	const std::map<int, std::string>&			getErrorPages() const;
	uint64_t									getClientMaxBodySize() const;
	const std::map<std::string, LocationBlock>&	getLocations() const;


	friend std::ostream& operator<<(std::ostream& out, ServerBlock& server) {
		out << "Server:" << std::endl;
		out << "\t" << "version: HTTP/" << server.getMajorVersion() << "." << server.getMinorVersion() << std::endl;
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
		std::map<std::string, LocationBlock> locations = server.getLocations();
		for (std::map<std::string, LocationBlock>::iterator i = locations.begin(); i != locations.end(); i++) {
			out << "\t" << "Location: " << (*i).first << std::endl;
			std::cout << (*i).second;
		}
		return out;
	}

};

#endif
