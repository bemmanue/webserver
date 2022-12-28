#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include <map>

#include "LocationBlock.hpp"

class ServerBlock {
public:
	typedef std::map<int, std::string>				ErrorPages;
	typedef std::map<std::string, LocationBlock>	Locations;

private:
	std::string					_listen;
	std::string					_server_name;
	std::vector<std::string>	_error_pages;
	std::string					_client_max_body_size;
	std::vector<LocationBlock>	_location;
//	std::string		_host;
//	int				_port;
//	std::string		_server_name;
//	ErrorPages		_error_pages;
//	int				_client_max_body_size;
//	Locations		_locations;

public:
	void setListen(const std::vector<std::string>& value);
	void setServerName(const std::vector<std::string>& value);
	void setErrorPages(const std::vector<std::string>& value);
	void setClientMaxBodySize(const std::vector<std::string>& value);
	void setLocation(const LocationBlock& value);

	void print();
};

#endif //SERVER_BLOCK_HPP
