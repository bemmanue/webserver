#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>


class Location {
public:
	typedef std::vector<std::string>			AcceptedMethods;
	typedef std::map<std::string, std::string>	CGIs;

private:
	std::string		_path;
	AcceptedMethods	_accepted_methods;
	std::string		_redirect;
	std::string		_root;
	bool			_autoindex;
	std::string 	_index;
	CGIs			_CGIs;

};


#endif //LOCATION_HPP
