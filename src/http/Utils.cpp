#include "Utils.hpp"

std::string	capitalize(const std::string& str) {
	std::string ret(str);

	ret[0] = (char)toupper((int)ret[0]);

	for (size_t i = 0; i + 1 < ret.size(); i++) {
		if (isalpha(ret[i])) {
			ret[i+1] = (char)tolower((int)ret[i+1]);
		} else {
			ret[i+1] = (char)toupper((int)ret[i+1]);
		}
	}

	return ret;
}

std::string	readToken(const std::string& str, size_t* i) {
//	token = 1*tchar
	std::string token;

	while (*i < str.size()) {
		if (isTchar(str[*i])) {
			token.push_back(str[(*i)++]);
		} else {
			break;
		}
	}
	return token;
}

std::string	readQuotedString(const std::string& str, size_t* i) {
//	quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
//	qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
//	obs-text = %x80-FF
//	quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
	std::string quotedString;
	size_t		temp = *i;

	if (!skipRequiredChar(str, &temp, '"')) {
		return "";
	} else {
		quotedString.push_back('"');
	}

	while (temp < str.size()) {
		if (isQuotedText(str[temp])) {
			temp += 1;
		} else if (isQuotedPair(str.substr(temp, 2))) {
			temp += 2;
		} else {
			break;
		}
	}

	if (!skipRequiredChar(str, &temp, '"')) {
		return "";
	} else {
		quotedString.push_back('"');
		*i = temp;
	}

	return quotedString;
}

std::string	readDecNum(const std::string& str, size_t* i) {
	std::string num;

	while (*i < str.size()) {
		if (isdigit(str[*i])) {
			num.push_back(str[(*i)++]);
		} else {
			break;
		}
	}
	return num;
}

std::string	readVersion(const std::string& request, size_t* i) {
//	HTTP-version = HTTP-name "/" DIGIT "." DIGIT
//	HTTP-name = %x48.54.54.50 ; "HTTP", case-sensitive
	std::string version;

	version = request.substr(*i, 8);
	*i += 8;

	if (version.size() == 8 &&
		!version.compare(0, 5, "HTTP/") &&
		isdigit(version[5]) && version[6] == '.' && isdigit(version[7])) {
		return version;
	} else {
		return "";
	}
}

std::string readSegment(const std::string &request, size_t* i) {
//	segment = *( pchar / pct-encoded )
	std::string segment;

	while (request[*i]) {
		if (isPchar(request[*i])) {
			segment.push_back(request[(*i)++]);
		} else if (isPctEncoded(request.substr(*i, 3))) {
			segment.append(request.substr(*i, 3));
			i += 3;
		} else {
			break;
		}
	}
	return segment;
}

std::string readFieldValue(const std::string &request, size_t* i) {
//	field-value = *( field-content / obs-fold )
//	field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
	std::string fieldValue;

	while (isFieldVchar(request[*i]) || request[*i] == '\t') {
		if (request[*i] == ' ' || request[*i] == '\t') {
			size_t temp = *i;
			skipOWS(request, &temp);
			if (!isFieldVchar(request[temp])) {
				break;
			} else {
				fieldValue.push_back(' ');
				*i = temp;
			}
		} else {
			fieldValue.push_back(request[(*i)++]);
		}
	}
	return fieldValue;
}

std::string	readWord(const std::string& str, size_t* i) {
	std::string word;

	while (*i < str.size()) {
		if (isVchar(str[*i]) && str[*i] != ' ') {
			word.push_back(str[(*i)++]);
		} else {
			break;
		}
	}
	return word;
}

std::string readScheme(const std::string& str, size_t* i) {
//	scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	std::string scheme;

	if (*i < str.size() && isalpha(str[*i])) {
		scheme.push_back(str[(*i)++]);
	} else {
		return scheme;
	}

	while (*i < str.size()) {
		if (isalpha(str[*i]) || isdigit(str[*i]) || str[*i] == '+' || str[*i] == '-' || str[*i] == '.') {
			scheme.push_back(str[(*i)++]);
		} else {
			break;
		}
	}
	return scheme;
}

std::string readUserInfo(const std::string& str, size_t* i) {
//	userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
	std::string userinfo;

	while (*i < str.size()) {
		if (isUnreserved(str[*i]) || isSubDelim(str[*i]) || str[*i] == ':') {
			userinfo.push_back(str[*i]);
			*i += 1;
		} else if (isPctEncoded(str.substr(*i, 3))) {
			*i += 3;
		} else {
			break;
		}
	}
	return userinfo;
}

std::string readIPvFuture(const std::string& str, size_t* i) {
//	IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	std::string host;

	if (str[*i] == '[') {
		host.push_back(str[(*i)++]);
		if (str[*i] == 'v') {
			host.push_back(str[(*i)++]);
			while (*i < str.size()) {
				if (isUnreserved(str[*i]) || isSubDelim(str[*i]) || str[*i] == ':') {
					host.push_back(str[(*i)++]);
				} else if (str[*i] == ']') {
					host.push_back(str[(*i)++]);
					break;
				} else {
					break;
				}
			}
		}
	}
	return host;
}

int	readPort(const std::string& str, size_t* i) {
//	port = *DIGIT
	std::string port_s;
	int			port;

	port_s = readDecNum(str, i);
	if (port_s.empty()) {
		return -1;
	}

	port_s = port_s.substr(port_s.find_first_not_of('0'));
	if (port_s.size() > 5) {
		return -1;
	}

	port = (int)strtol(port_s.c_str(), nullptr, 10);
	if (port > 65536) {
		return -1;
	}

	return port;
}

std::string readIPv6address(const std::string& str, size_t* i) {
//	IPv6address =                6( h16 ":" ) ls32
//	/                       "::" 5( h16 ":" ) ls32
//	/               [ h16 ] "::" 4( h16 ":" ) ls32
//	/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//	/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//	/ [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32
//	/ [ *4( h16 ":" ) h16 ] "::" ls32
//	/ [ *5( h16 ":" ) h16 ] "::" h16
//	/ [ *6( h16 ":" ) h16 ] "::"
//	ls32 = ( h16 ":" h16 ) / IPv4address;
//	h16 = 1*4HEXDIG;
	std::string host;

	if (str[*i] == '[') {
		host.push_back(str[(*i)++]);
		while (*i < str.size()) {
			if (ishexnumber(str[*i]) || str[*i] == ':' || str[*i] == '.') {
				host.push_back(str[(*i)++]);
			} else if (str[*i] == ']') {
				host.push_back(str[(*i)++]);
				break;
			} else {
				break;
			}
		}
	}
	return host;
}

std::string readIPLiteral(const std::string& str, size_t* i) {
//	IP-literal = "[" ( IPv6address / IPvFuture ) "]"
	std::string host;

	if (str[*i] == '[') {
		if (str[*i + 1] == 'v') {
			host = readIPvFuture(str, i);
		} else {
			host = readIPv6address(str, i);
		}
	}
	return host;
}

std::string readHost(const std::string& str, size_t* i) {
//	host = IP-literal / IPv4address / reg-name
//	IP-literal = "[" ( IPv6address / IPvFuture ) "]"
//	IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
//	reg-name = *( unreserved / pct-encoded / sub-delims )
	std::string host;

	// read IP-literal
	if (str[*i] == '[') {
		host = readIPLiteral(str, i);
	} else {
		// read IPv4address or reg-name
		while (*i < str.size()) {
			if (isUnreserved(str[*i]) || isSubDelim(str[*i])) {
				host.push_back(str[*i]);
				*i += 1;
			} else if (isPctEncoded(str.substr(*i, 3))) {
				*i += 3;
			} else {
				break;
			}
		}
	}
	return host;
}

std::string	readPathAbempty(const std::string& str, size_t* pos) {
//	path-abempty  = *( "/" segment )
	std::string path;
	std::string segment;

	while (str[*pos] == '/') {
		path.push_back(str[(*pos)++]);
		segment = readSegment(str, pos);
		path.append(segment);
	}
	return path;
}

std::string	readPathAbsolute(const std::string &str, size_t* pos) {
//	absolute-path = 1*( "/" segment )
	return readPathAbempty(str, pos);
}

std::string	readPathRootless(const std::string& str, size_t* i) {
//	path-rootless = segment-nz *( "/" segment )
//	segment-nz = 1*pchar
	std::string path;

	while (*i < str.size()) {
		if (isPchar(str[*i])) {
			path.push_back(str[*i]);
			*i += 1;
		} else if (isPctEncoded(str.substr(*i, 3))) {
			path.append(str.substr(*i, 3));
			*i += 3;
		} else {
			break;
		}
	}

	if (path.empty()) {
		return path;
	}

	return path.append(readPathAbempty(str, i));
}

std::string	readQuery(const std::string &str, size_t* i) {
//	query = *( pchar / pct-encoded / "/" / "?" )
	std::string query;

	while (*i < str.size()) {
		if (isPchar(str[*i]) || str[*i] == '/' || str[*i] == '?') {
			query.push_back(str[*i]);
			*i += 1;
		} else if (isPctEncoded(str.substr(*i, 3))) {
			query.append(str.substr(*i, 3));
			*i += 3;
		} else {
			break;
		}
	}
	return query;
}

std::string	readFragment(const std::string& str, size_t* i) {
//	fragment = *( pchar / pct-encoded / "/" / "?" )
	std::string fragment;

	while (*i < str.size()) {
		if (isPchar(str[*i]) || str[*i] == '/' || str[*i] == '?') {
			fragment.push_back(str[*i]);
			*i += 1;
		} else if (isPctEncoded(str.substr(*i, 3))) {
			fragment.append(str.substr(*i, 3));
			*i += 3;
		} else {
			break;
		}
	}
	return fragment;
}

std::string	readChunkData(const std::string& str, size_t* i, size_t chunkSize) {
//	chunk-data = 1*OCTET
	std::string data;

	chunkSize += *i;
	while (str[*i] && *i < chunkSize) {
		data.push_back(str[(*i)++]);
	}
	return data;
}

long	readChunkSize(const std::string& str, size_t* i) {
//	chunk-size = 1*HEXDIG
	std::string size;

	while (ishexnumber(str[*i])) {
		size.push_back(str[(*i)++]);
	}

	long res = strtol(size.c_str(), NULL, 16);
	if (!res && (errno == EINVAL || errno == ERANGE)) {
		return -1;
	}

	return res;
}

void	skipOWS(const std::string &request, size_t *i) {
//	OWS = *( SP / HTAB )
	if (*i > request.size()) {
		return ;
	}

	while (request[*i] == ' ' || request[*i] == '\t') {
		++*i;
	}
}

bool	skipCRLF(const std::string &request, size_t *i) {
//	CRLF = \r\n
	if (*i > request.size()) {
		return false;
	}

	if (request[*i] == '\r' && request[*i + 1] == '\n') {
		*i += 2;
		return true;
	}
	return false;
}

bool	skipRequiredChar(const std::string &request, size_t *i, char c) {
	if (*i > request.size()) {
		return false;
	}

	if (request[*i] == c) {
		++*i;
		return true;
	}
	return false;
}

bool	isTchar(char a) {
//	tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
	const char *set = "!#$%&'*+-.^_`|~";

	if (isalpha(a) || isdigit(a) || std::strchr(set, a)) {
		return true;
	}
	return false;
}

bool	isPchar(char a) {
//	pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
	if (isUnreserved(a) || isSubDelim(a) || a == ':' || a == '@') {
		return true;
	}
	return false;
}

bool	isVchar(char a) {
//	vchar = any visible ASCII symbol
	if (isprint(a)) {
		return true;
	}
	return false;
}

bool	isReserved(char a) {
	//	reserved = gen-delims / sub-delims
	if (isGenDelim(a) || isSubDelim(a)) {
		return true;
	}
	return false;
}

bool	isUnreserved(char a) {
//	unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
	if (isalpha(a) || isdigit(a) || std::strchr("-._~", a)) {
		return true;
	}
	return false;
}

bool	isGenDelim(char a) {
//	gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
	if (std::strchr(":/?#[]@", a)) {
		return true;
	}
	return false;
}

bool	isSubDelim(char a) {
//	sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
	if (std::strchr("!$&'()*+,;=", a)) {
		return true;
	}
	return false;
}

bool	isObsText(unsigned char a) {
//	obs-text = %x80-FF
	if (a >= 0x80 && a <= 0xFF) {
		return true;
	}
	return false;
}

bool	isFieldVchar(char a) {
//	field-vchar = vchar / obs-text
	if (isVchar(a) || isObsText(a)) {
		return true;
	}
	return false;
}

bool	isQuotedText(char a) {
//	qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
	if (a == '\t' || a == ' ' || a == 0x21 || (a >= 0x23 && a <= 0x5B) || (a >= 0x5D && a <= 0x7E) || isObsText(a)) {
		return true;
	}
	return false;
}

bool	isQuotedPair(const std::string& str) {
//	quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
	if (str.size() == 2 && str[0] == '\\' && (str[1] == '\t' || str[1] == ' ' || isVchar(str[1])) || isObsText(str[1])) {
		return true;
	}
	return false;
}

bool	isPctEncoded(const std::string& str) {
//	pct-encoded = "%" HEXDIG HEXDIG
	if (str.size() == 3 && str[0] == '%' && ishexnumber(str[1]) && ishexnumber(str[2])) {
		return true;
	}
	return false;
}

bool	isValidHost(const std::string& str) {
//	host = IP-literal / IPv4address / reg-name
	if (isIPLiteral(str) || isIPv4address(str) || isRegName(str)) {
		return true;
	}
	return false;
}

bool	isIPLiteral(const std::string& str) {
//	IP-literal = "[" ( IPv6address / IPvFuture ) "]"
	std::string address;

	if (str.front() == '[' && str.back() == ']') {
		address = str.substr(1, str.size()-2);
		if (isIPv6address(address) || isIPvFuture(address)) {
			return true;
		}
	}
	return false;
}

bool	isIPv4address(const std::string& str) {
//	IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
	struct in_addr addr = {};
	return inet_pton(PF_INET, str.c_str(), &addr);
}

bool	isIPv6address(const std::string& str) {
//	IPv6address =                6( h16 ":" ) ls32
//	/                       "::" 5( h16 ":" ) ls32
//	/               [ h16 ] "::" 4( h16 ":" ) ls32
//	/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//	/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//	/ [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32
//	/ [ *4( h16 ":" ) h16 ] "::" ls32
//	/ [ *5( h16 ":" ) h16 ] "::" h16
//	/ [ *6( h16 ":" ) h16 ] "::"
//	ls32 = ( h16 ":" h16 ) / IPv4address;
//	h16 = 1*4HEXDIG;
	struct in6_addr addr = {};
	return inet_pton(PF_INET6, str.c_str(), &addr);
}

bool	isIPvFuture(const std::string& str) {
//	IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	size_t i = 0;

	if (!skipRequiredChar(str, &i, 'v') || !ishexnumber(str[i])) {
		return false;
	}
	while (i < str.size() && ishexnumber(str[i])) {
		i++;
	}
	if (!skipRequiredChar(str, &i, '.') || (!isUnreserved(str[i]) && !isSubDelim(str[i]) && str[i] != ':')) {
		return false;
	}
	while (i < str.size() && (isUnreserved(str[i]) || isSubDelim(str[i]) || str[i] == ':')) {
		i++;
	}
	if (str[i] != '\0') {
		return false;
	}
	return true;
}

bool	isRegName(const std::string& str) {
//	reg-name = *( unreserved / pct-encoded / sub-delims )
	size_t i = 0;

	while (i < str.size()) {
		if (isUnreserved(str[i]) || isSubDelim(str[i])) {
			i += 1;
		} else if (isPctEncoded(str.substr(3))) {
			i += 3;
		} else {
			return false;
		}
	}
	return true;
}

bool	isOriginForm(const std::string& str) {
//	origin-form = absolute-path [ "?" query ]
	size_t i = 0;

	if (readPathAbsolute(str, &i).empty()) {
		return false;
	}
	if (skipRequiredChar(str, &i, '?')) {
		readQuery(str, &i);
	}
	if (str[i] != '\0') {
		return false;
	}
	return true;
}

bool	resourceExists(const std::string& filename) {
	struct stat state;

	return (stat(filename.c_str(), &state) == 0);
}

bool	isFile(const std::string& filename) {
	struct stat state;

	if (stat(filename.c_str(), &state) < 0) {
		return false;
	}
	return S_ISREG(state.st_mode);
}

bool	isDirectory(const std::string& dirname) {
	struct stat state;

	if (stat(dirname.c_str(), &state) < 0) {
		return false;
	}
	return S_ISDIR(state.st_mode);
}

bool	isHTTPMethod(const std::string& method) {
	if (method == GET		||
		method == POST		||
		method == PUT		||
		method == HEAD		||
		method == DELETE	||
		method == CONNECT	||
		method == OPTIONS	||
		method == TRACE		||
		method == PATCH) {
		return true;
	}
	return false;
}

int	toDigit(char c) {
	if (isdigit(c)) {
		return c - '0';
	} else {
		return -1;
	}
}
