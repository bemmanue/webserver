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

std::string	readToken(const std::string& request, size_t* i) {
//	token = 1*tchar
	std::string token;

	while (isTchar(request[*i])) {
		token.push_back(request[(*i)++]);
	}
	return token;
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

std::string	readAbsolutePath(const std::string &request, size_t* i) {
//	absolute-path = 1*( "/" segment )
	std::string path;
	std::string segment;

	while (request[*i] == '/') {
		path.push_back(request[(*i)++]);
		segment = readSegment(request, i);
		path.append(segment);
	}
	return path;
}

std::string	readQuery(const std::string &request, size_t* i) {
//	query = *( pchar / pct-encoded / "/" / "?" )
	std::string query;

	while (isPchar(request[*i]) || request[*i] == '/' || request[*i] == '?') {
		query.push_back(request[(*i)++]);
	}
	return query;
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
//	pchar = unreserved / sub-delims / ":" / "@"
	if (isUnreserved(a) || isSubDelim(a) || std::strchr(":@", a)) {
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

bool	isUnreserved(char a) {
//	unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
	if (isalpha(a) || isdigit(a) || std::strchr("-._~", a)) {
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

bool	isFieldVchar(char a) {
//	field-vchar = vchar / obs-text
	if (isVchar(a) || a > 127) {
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

bool	isEmptyLine(const std::string& request, size_t pos) {
	if (request[pos] == '\r' && request[pos+1] == '\n') {
		return true;
	}
	return false;
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
