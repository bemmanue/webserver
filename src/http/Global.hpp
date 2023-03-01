#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>

#define HTTP_MAJOR_VERSION	1
#define HTTP_MINOR_VERSION	1

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

#ifndef ALLOWED_METHODS
	# define GET		"GET"
	# define POST		"POST"
	# define DELETE		"DELETE"
#endif

#define SERVER_ROOT 	"data"


bool	isHTTPMethod(const std::string& method);


#endif //GLOBAL_HPP
