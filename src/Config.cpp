#include <fstream>
#include "../include/Config.hpp"

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	std::vector<char> buffer;

	buffer.resize(size);
	file.seekg(0, std::ios::beg);
	file.read(buffer.data(), size);
	return buffer;
}

std::string getDirective(const std::vector<char>& buffer, int i) {
	std::string directive;

	for ( ; i < buffer.size(); ++i) {
		if (isspace(buffer[i]) || buffer[i] == ';') {
			break;
		}
		directive.push_back(buffer[i]);
	}
	return directive;
}

int getServerBlock(const std::vector<char>& buffer, int i) {
	for ( ; i < buffer.size(); ++i) {
		if (isspace(buffer[i])) {
			continue;
		}
//		if ()
//		directive.push_back(buffer[i]);
	}
	return i;
}

Config*	parseConfig(const std::string& filename) {
	std::vector<char>	buffer;
	std::string			directive;

	buffer = readFile(filename);

	for (int i = 0; i < buffer.size(); ++i) {
		if (isspace(buffer[i])) {
			continue;
		}

		directive = getDirective(buffer, i);

		if (directive == KW_SERVER) {
			i = getServerBlock();
		} else {
			throw std::exception();
		}
		std::cout << buffer[i];
	}
	return nullptr;
}