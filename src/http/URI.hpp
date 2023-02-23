#ifndef URI_HPP
#define URI_HPP

#include <iostream>
#include <algorithm>
#include "Utils.hpp"

class URI {
private:
    std::string		_scheme;

	std::string		_authority;
	std::string		_userinfo;
    std::string		_host;
    int				_port;

    std::string		_path;
    std::string		_query;
    std::string		_fragment;

	std::string		_raw;
	bool			_correct;

	typedef std::string::iterator iter_t;

public:
    URI();
    URI(const URI& other);
    URI& operator=(const URI& other);
    ~URI();

	void				parseURI(const std::string& raw);
	void				parseHost(const std::string& raw);
    static std::string	URLencode(const std::string &);
    static std::string	URLdecode(const std::string &);
    std::string			getAuthority() const;
    bool				isCorrect() const;
    bool				hasPort() const;

	friend std::ostream& operator<<(std::ostream& out, URI& uri) {
		out << "scheme: " << uri._scheme << std::endl;
		out << "authority: " << uri.getAuthority() << std::endl;
		out << "userinfo: " << uri._userinfo << std::endl;
		out << "host: " << uri._host << std::endl;
		out << "port: " << uri._port << std::endl;
		out << "path: " << uri._path << std::endl;
		out << "query: " << uri._query << std::endl;
		out << "fragment: " << uri._fragment << std::endl;
		out << "correct: " << std::boolalpha << uri._correct << std::endl;
		return out;
	}
};


#endif
