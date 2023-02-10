#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include <set>

#define HTTP_VERSION	"HTTP/1.1"

#define SERVER_ROOT		"data"

#ifndef ALLOWED_METHODS
	# define GET		"GET"
	# define POST		"POST"
	# define DELETE		"DELETE"
#endif

#ifndef HTTP_METHODS
	# define GET		"GET"
	# define POST		"POST"
	# define PUT		"PUT"
	# define HEAD		"HEAD"
	# define DELETE		"DELETE"
	# define CONNECT	"CONNECT"
	# define OPTIONS	"OPTIONS"
	# define TRACE		"TRACE"
	# define PATCH		"PATCH"
#endif


class Global {
private:
	std::set<std::string>	_allowedMethods;	// methods maintained by this server
	std::set<std::string>	_httpMethods;		// all methods

public:
	Global();
	~Global();

	bool	isAllowedMethod(const std::string& method) const;
	bool	isHTTPMethod(const std::string& method) const;
	bool	isServerVersion(const std::string& version) const;
	bool	isSupportedVersion(const std::string& version) const;
};

extern const Global global;


#endif //GLOBAL_HPP
