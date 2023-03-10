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

std::string toHexString(int num) {
	std::stringstream stream;
	stream << std::hex << num;
	return stream.str();
}

bool	resourceExists(const std::string& filename) {
	struct stat state{};

	return (stat(filename.c_str(), &state) == 0);
}

bool	isFile(const std::string& filename) {
	struct stat state{};

	if (stat(filename.c_str(), &state) < 0) {
		return false;
	}
	return S_ISREG(state.st_mode);
}

bool	isDirectory(const std::string& dirname) {
	struct stat state{};

	if (stat(dirname.c_str(), &state) < 0) {
		return false;
	}
	return S_ISDIR(state.st_mode);
}
