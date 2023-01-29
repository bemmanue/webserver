#pragma once


class ConfigException : public std::exception {
public:
	explicit ConfigException(const std::string& msg) {
		_message = new std::string(msg);
	}
	explicit ConfigException(const std::string& msg, size_t line) {
		_message = new std::string(msg + " in line " + std::to_string(line));
	}
	virtual ~ConfigException() throw() { delete _message; }
	virtual const char* what() const throw() { return _message->c_str(); }

protected:
	std::string*	_message;
};

class ClientMaxBodySizeConfigException : public ConfigException {
public:
	explicit ClientMaxBodySizeConfigException(const std::string& value, size_t line) :
	ConfigException("invalid value \"" + value + "\"", line) {}
};

class InvalidErrorPageConfigException : public ConfigException {
public:
	explicit InvalidErrorPageConfigException(const std::string& value, size_t line) :
	ConfigException("value \"" + value + "\" must be between 300 and 599", line) {}
};

class InvalidAutoindexValueConfigException : public ConfigException {
public:
	explicit InvalidAutoindexValueConfigException(const std::string& value, size_t line) :
		ConfigException("invalid value \"" + value + " in \"autoindex\" directive, it must be \"on\" or \"off\"", line) {}
};


class InvalidPortConfigException : public ConfigException {
public:
	explicit InvalidPortConfigException(const std::string& port, size_t line) :
		ConfigException("invalid port \"" + port + "\"", line) {}
};


class HostNotFoundConfigException : public ConfigException {
public:
	explicit HostNotFoundConfigException(const std::string& host, size_t line) :
		ConfigException("host \"" + host + "\" not found", line) {}
};


class EmptyFileConfigException : public ConfigException {
public:
	explicit EmptyFileConfigException() :
		ConfigException("configuration file is empty") {}
};


class UnexpectedEndOfFileConfigException : public ConfigException {
public:
	explicit UnexpectedEndOfFileConfigException(size_t line) :
		ConfigException("unexpected end of file, expecting \"}\"", line) {}
};


class NotAllowedDirectiveConfigException : public ConfigException {
public:
	explicit NotAllowedDirectiveConfigException(const std::string& directive, size_t line) :
		ConfigException("directive \"" + directive + "\" is not allowed here", line){}
};


class InvalidMethodConfigException : public ConfigException {
public:
	explicit InvalidMethodConfigException(const std::string& method, size_t line) :
		ConfigException("invalid method \"" + method + "\"", line){}
};


class UnknownDirectiveConfigException : public ConfigException {
public:
	explicit UnknownDirectiveConfigException(const std::string& directive, size_t line) :
		ConfigException("unknown directive \"" + directive + "\"", line) {}
};


class UnexpectedTokenConfigException : public ConfigException {
public:
	explicit UnexpectedTokenConfigException(const std::string& token, size_t line) :
		ConfigException("unexpected token \"" + token + "\"", line) {}
};


class NoOpeningBraceConfigException : public ConfigException {
public:
	explicit NoOpeningBraceConfigException(const std::string& directive, size_t line) :
		ConfigException("directive \"" + directive + "\" has no opening \"{\"", line) {}
};


class InvalidNumberOfArgumentsConfigException : public ConfigException {
public:
	explicit InvalidNumberOfArgumentsConfigException(const std::string& directive, size_t line) :
		ConfigException("invalid number of arguments in \"" + directive + "\"", line) {}
};

