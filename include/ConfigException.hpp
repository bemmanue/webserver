#ifndef CONFIG_EXCEPTION_HPP
#define CONFIG_EXCEPTION_HPP

class ConfigException : public std::exception {
public:
	explicit ConfigException(const std::string& message, size_t line):
			_message(message + " in " +  std::to_string(line)) {}
	virtual ~ConfigException() throw() {}

	virtual const char* what() const throw() {
		return _message.c_str();
	}
private:
	std::string _message;
};


class UnknownDirectiveConfigException : public std::exception {
public:
	explicit UnknownDirectiveConfigException(const std::string& directive, size_t line)
		: _directive(directive), _line(line) {}

	virtual ~UnknownDirectiveConfigException() throw() {}

	virtual const char* what() const throw() {
		std::string msg;

		msg = "unknown directive \"" + _directive + "\" in line " + std::to_string(_line);
		return msg.c_str();
	}

private:
	std::string	_directive;
	size_t		_line;
};


class UnexpectedTokenConfigException : public std::exception {
public:
	explicit UnexpectedTokenConfigException(const std::string& token, size_t line)
		: _token(token), _line(line) {}

	virtual ~UnexpectedTokenConfigException() throw() {}

	virtual const char* what() const throw() {
		std::string msg;

		msg = "unexpected token \"" + _token + "\" in line " + std::to_string(_line);
		return msg.c_str();
	}

private:
	std::string _token;
	size_t		_line;
};


class NoOpeningBraceConfigException : public std::exception {
public:
	explicit NoOpeningBraceConfigException(const std::string& directive, size_t line)
		: _directive(directive), _line(line) {}

	virtual ~NoOpeningBraceConfigException() throw() {}

	virtual const char* what() const throw() {
		std::string msg;

		msg = "directive \"" + _directive + "\" has no opening \"{\" in line " + std::to_string(_line);
		return msg.c_str();
	}

private:
	std::string _directive;
	size_t		_line;
};

class UnexpectedEndOfFileConfigException : public std::exception {
public:
	explicit UnexpectedEndOfFileConfigException(size_t line)
			: _line(line) {}

	virtual ~UnexpectedEndOfFileConfigException() throw() {}

	virtual const char* what() const throw() {
		std::string msg;

		msg = "unexpected end of file, expecting \"}\" in line" + std::to_string(_line);
		return msg.c_str();
	}

private:
	size_t		_line;
};


#endif //CONFIG_EXCEPTION_HPP
