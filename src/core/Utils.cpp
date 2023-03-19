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

template <typename TP>
std::time_t to_time_t(TP tp) {
	auto time = tp - TP::clock::now() + std::chrono::system_clock::now();
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(time);

	return std::chrono::system_clock::to_time_t(sctp);
}

std::string timeToString(std::filesystem::file_time_type point) {
	std::time_t			tt = to_time_t(point);
	std::tm				*gmt = std::gmtime(&tt);
	std::stringstream	buffer;

	buffer << std::put_time(gmt, "%d-%b-%Y %H:%M");
	return buffer.str();
}
