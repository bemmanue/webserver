#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <map>
#include <set>
#include <arpa/inet.h>
#include <netdb.h>

#include "LocationBlock.hpp"

#define DEFAULT_PORT 					8080
#define DEFAULT_HOST 					"127.0.0.1"
#define DEFAULT_CLIENT_MAX_BODY_SIZE	1000000


class ServerBlock {
private:
	std::string     						_host;					//ok
	size_t            						_port;					//ok
	std::set<std::string>					_server_names;			//ok
	std::map<int, std::string>				_error_pages;			//ok
	uint64_t 								_client_max_body_size;	//ok
	std::map<std::string, LocationBlock>	_locations;				//ok

public:
	ServerBlock();
	~ServerBlock();

	void setHost(const std::string& addr);
	void setPort(size_t port);
	void setServerName(const std::string& name);
	void setErrorPages(int code, const std::string& path);
	void setClientMaxBodySize(uint64_t bytes);
	void setLocation(const std::string& path, const LocationBlock& location);

	bool hasLocation(const std::string& location);

	std::string								getHost() const;
	size_t									getPort() const;
	std::set<std::string>					getServerNames() const;
	std::map<int, std::string>				getErrorPages() const;
	uint64_t								getClientMaxBodySize() const;
	std::map<std::string, LocationBlock>	getLocations() const;

	void print();	//debug
};

#endif //SERVER_BLOCK_HPP
