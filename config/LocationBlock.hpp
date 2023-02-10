#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "Global.hpp"


class LocationBlock {
private:
	std::string							_path;				//ok
	bool								_autoindex;			//ok
	std::map<std::string, std::string>	_CGIs;				//ok
	std::vector<std::string>			_index;				//ok
	std::set<std::string>				_methods_allowed;	//ok
	std::map<int, std::string>			_redirect;			//ok
	std::string							_root;				//ok

public:
	LocationBlock();
	~LocationBlock();

	void setPath(const std::string& parameter);
	void setAutoindex(bool status);
	void setCGIs(const std::string& extension, const std::string& path);
	void setIndex(const std::vector<std::string>& parameter);
	void setMethodsAllowed(const std::string& method);
	void setRedirect(int code, const std::string& uri);
	void setRoot(const std::string& path);

	std::string							getPath() const;
	bool								getAutoindex() const;
	std::map<std::string, std::string>	getCGIs() const;
	std::vector<std::string>			getIndices() const;
	std::set<std::string>				getMethodsAllowed() const;
	std::map<int, std::string>			getRedirect() const;
	std::string							getRoot() const;

	void print();	//debug
};


#endif //LOCATION_BLOCK_HPP
