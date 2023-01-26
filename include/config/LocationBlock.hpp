#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>


class LocationBlock {
public:
	typedef std::vector<std::string>			AcceptedMethods;
	typedef std::map<std::string, std::string>	CGIs;
	typedef struct s_Redirect {
		int			code;
		std::string	uri;
	}	Redirect;

//private:
	std::string					_path;
	bool						_autoindex;
	CGIs						_CGIs;
	std::vector<std::string>	_index;
	AcceptedMethods				_methods_allowed;
	Redirect					_redirect;
	std::string					_root;

public:
	void setPath(const std::string& parameter);
	void setAutoindex(bool status);
	void setCGIs(const std::string& extension, const std::string& path);
	void setIndex(const std::vector<std::string>& parameter);
	void setMethodsAllowed(const std::string& method);
	void setRedirect(const std::string& code, const std::string& uri);
	void setRoot(const std::string& path);

};


#endif //LOCATION_BLOCK_HPP
