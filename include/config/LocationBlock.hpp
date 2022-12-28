#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>


class LocationBlock {
public:
	typedef std::vector<std::string>			AcceptedMethods;
	typedef std::map<std::string, std::string>	CGIs;

//private:
	std::string					_path;
	std::string					_autoindex;
	std::vector<std::string>	_CGIs;
	std::string 				_index;
	std::vector<std::string>	_methods_allowed;
	std::vector<std::string>	_redirect;
	std::string					_root;
//	std::string		_path;
//	AcceptedMethods	_accepted_methods;
//	std::string		_redirect;
//	std::string		_root;
//	bool			_autoindex;
//	std::string 	_index;
//	CGIs			_CGIs;

public:
	void setPath(const std::string& parameter);
	void setAutoindex(const std::vector<std::string>& parameter);
	void setCGIs(const std::vector<std::string>& parameter);
	void setIndex(const std::vector<std::string>& parameter);
	void setMethodsAllowed(const std::vector<std::string>& parameter);
	void setRedirect(const std::vector<std::string>& parameter);
	void setRoot(const std::vector<std::string>& parameter);

};


#endif //LOCATION_BLOCK_HPP
