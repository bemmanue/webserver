#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "../http/URI.hpp"
#include "../http/Global.hpp"

#define DEFAULT_INDEX	"index.html"
#define DEFAULT_ROOT	"html"


class LocationConfig {
private:
	std::string							_path;
	bool								_autoindex;
	std::map<std::string, std::string>	_CGIs;
	std::vector<std::string>			_index;
	std::set<std::string>				_limitExcept;
	std::map<int, std::string>			_redirect;
	std::string							_root;
	std::set<std::string>				_methodsAllowed;

public:
	LocationConfig();
	LocationConfig(const LocationConfig& other);
	LocationConfig& operator=(const LocationConfig& other);
	~LocationConfig();

	void	setPath(const std::string& parameter);
	void	setAutoindex(bool status);
	void	setCGIs(const std::string& extension, const std::string& path);
	void	setIndex(const std::vector<std::string>& parameter);
	void	setLimitExcept(const std::string& method);
	void	setRedirect(int code, const std::string& uri);
	void	setRoot(const std::string& path);

	bool	isMethodAllowed(const std::string& method);

	std::string							getPath() const;
	bool								getAutoindex() const;
	std::map<std::string, std::string>	getCGIs() const;
	std::vector<std::string>			getIndices() const;
	std::set<std::string>				getMethodsAllowed() const;
	std::map<int, std::string>			getRedirect() const;
	std::string							getRoot() const;


	friend std::ostream& operator<<(std::ostream& out, LocationConfig& l) {
		out << "\t\t" << "path: " << l.getPath() << std::endl;
		out << "\t\t" << "autoindex: " << std::boolalpha << l.getAutoindex() << std::endl;
		std::map<std::string, std::string> cgi = l.getCGIs();
		for (std::map<std::string, std::string>::iterator i = cgi.begin(); i != cgi.end(); i++) {
			out << "\t\t" << "cgi: " << (*i).first << " " << (*i).second << std::endl;
		}
		std::vector<std::string> indices = l.getIndices();
		for (int i = 0; i < indices.size(); i++) {
			out << "\t\t" << "index: " << indices[i] << std::endl;
		}
		std::set<std::string> allowed = l.getMethodsAllowed();
		for (std::set<std::string>::iterator i = allowed.begin(); i != allowed.end(); i++) {
			out << "\t\t" << "method_allowed: " << *i << std::endl;
		}
		std::map<int, std::string> redir = l.getRedirect();
		for (std::map<int, std::string>::iterator i = redir.begin(); i != redir.end(); i++) {
			out << "\t\t" << "error_page: " << (*i).first << " " << (*i).second << std::endl;
		}
		out << "\t\t" << "root: " << l.getRoot() << std::endl;
		return out;
	}

};

#endif
