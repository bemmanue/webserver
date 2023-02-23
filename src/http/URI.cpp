#include "URI.hpp"

URI::URI():
	_port(-1),
	_correct(true) {
}

URI::URI(const URI &other):
	_port(-1),
	_correct(true) {
	operator=(other);
}

URI &URI::operator=(const URI& other) {
	if (this != &other) {
		_scheme = other._scheme;
		_authority = other._authority;
		_userinfo = other._userinfo;
		_host = other._host;
		_port = other._port;
		_path = other._path;
		_query = other._query;
		_fragment = other._fragment;
		_raw = other._raw;
		_correct = other._correct;
	}
	return *this;
}

URI::~URI(void) {}

void URI::parseURI(const std::string& raw) {
//	URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
//	hier-part  = "//" authority path-abempty
//	/ path-absolute
//	/ path-rootless
//	/ path-empty
//	authority = [ userinfo "@" ] host [ ":" port ]
	size_t i = 0;

	_raw = raw;

	_scheme = readScheme(raw, &i);
	if (_scheme.empty()) {
		_correct = false;
		return;
	}

	if (!skipRequiredChar(raw, &i, ':')) {
		_correct = false;
		return;
	}

	// hier-part
	if (skipRequiredChar(raw, &i, '/')) {

		// authority path-abempty
		if (skipRequiredChar(raw, &i, '/')) {

			// userinfo
			size_t temp = i;
			_userinfo = readUserInfo(raw, &temp);
			if (!skipRequiredChar(raw, &temp, '@')) {
				_userinfo = "";
			} else {
				i = temp;
				_authority.append(_userinfo);
				_authority.push_back('@');
			}

			// host
			_host = readHost(raw, &i);
			if (!isValidHost(_host)) {
				_correct = false;
				return;
			}
			_authority.append(_host);

			// port
			if (skipRequiredChar(raw, &i, ':')) {
				if (isdigit(raw[i])) {
					_port = readPort(raw, &i);
					if (_port < 1) {
						_correct = false;
						return;
					}
					_authority.push_back(':');
					_authority.append(std::to_string(_port));
				}
			}

			// path-abempty
			_path = readPathAbempty(raw, &i);

		} else {
			// path-absolute
			_path = readPathAbsolute(raw, &i);
		}

	} else if (isPchar(raw[i]) || isPctEncoded(raw.substr(i, 3))) {
		// path-rootless
		_path = readPathRootless(raw, &i);
	} else {
		// path-empty
		_path = "";
	}

	// query
	if (skipRequiredChar(raw, &i, '?')) {
		_query = readQuery(raw, &i);
	}

	// fragment
	if (skipRequiredChar(raw, &i, '#')) {
		_fragment = readFragment(raw, &i);
	}

	if (raw[i] != '\0') {
		_correct = false;
	}
}

void URI::parseHost(const std::string &raw) {
//	host = uri-host [ ":" port ]
	size_t i = 0;

	// host
	_host = readHost(raw, &i);
	if (!isValidHost(_host)) {
		_correct = false;
		return;
	}
	_authority.append(_host);

	// port
	if (skipRequiredChar(raw, &i, ':')) {
		if (isdigit(raw[i])) {
			_port = readPort(raw, &i);
			if (_port < 1) {
				_correct = false;
				return;
			}
			_authority.push_back(':');
			_authority.append(std::to_string(_port));
		}
	}

	if (raw[i] != '\0') {
		_correct = false;
	}
}

std::string URI::URLencode(const std::string &s) {
    static const char shouldBeEncoded[256] = {
        /*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
        /* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,

        /* 4 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        /* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        /* 6 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        /* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,

        /* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        /* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    const char dec2hex[] = "0123456789ABCDEF";

    std::string result;
    result.reserve(s.length() * 3);
    for (std::size_t i = 0; i < s.length(); i++) {
        if (shouldBeEncoded[static_cast<unsigned char>(s[i])]) {
            result += s[i];
        } else {
            result += '%';
            result += dec2hex[static_cast<unsigned char>(s[i]) >> 4];
            result += dec2hex[static_cast<unsigned char>(s[i]) & 0x0F];
        }
    }

    return result;
}

std::string URI::URLdecode(const std::string &s) {
    static const char hex2dec[256] = {
        /*       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
        /* 0 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 1 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 2 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 3 */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,

        /* 4 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 5 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 6 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 7 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        /* 8 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* 9 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* A */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* B */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        /* C */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* D */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* E */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        /* F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    if (s.length() < 3) {
        return s;
    }

    std::string result;
    // abcedf 6
    result.reserve(s.length());
    std::size_t i;
    for (i = 0; i < s.length() - 2; i++) {
        if (static_cast<unsigned char>(s[i]) == '%') {
            char d1 = hex2dec[static_cast<unsigned char>(s[i + 1])];
            char d2 = hex2dec[static_cast<unsigned char>(s[i + 2])];
            if (d1 != -1 && d2 != -1) {
                result += (d1 << 4) + d2;
                i += 2;
                continue;
            }
        }
        result += s[i];
    }
    if (i != s.length()) {
        result += s[s.length() - 2];
        result += s[s.length() - 1];
    }
    return result;
}

bool	URI::isCorrect() const {
	return _correct;
}

std::string	URI::getAuthority(void) const {
	return _authority;
}

bool URI::hasPort() const {
	return (_port >= 0);
}
