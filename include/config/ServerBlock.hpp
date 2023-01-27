#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <map>
#include <set>
#include <arpa/inet.h>
#include <netdb.h>

#include "LocationBlock.hpp"

class ServerBlock {
private:
	std::string     			_addr;	//ok
	size_t            			_port;	//ok
	std::set<std::string>		_server_name;	//ok
	std::map<int, std::string>	_error_pages;	//ok
	uint64_t 					_client_max_body_size;	//ok
	std::vector<LocationBlock>	_location;	//ok

public:
	void setHost(const std::string& addr);
	void setPort(size_t port);
	void setServerName(const std::string& name);
	void setErrorPages(int code, const std::string& path);
	void setClientMaxBodySize(uint64_t bytes);
	void setLocation(const LocationBlock& value);

	void print();
};

#endif //SERVER_BLOCK_HPP
