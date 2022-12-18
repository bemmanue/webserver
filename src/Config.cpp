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

std::string getDirectiveName(const std::vector<char>& buffer, int i) {
	std::string name;

	for ( ; i < buffer.size(); ++i) {
		if (isspace(buffer[i]) || buffer[i] == ';') {
			break;
		}
		name.push_back(buffer[i]);
	}
	return name;
}

int getBlockDirective(const std::vector<char>& buffer, int i) {
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
	std::string			directiveName;
	std::string			directive;

	buffer = readFile(filename);

	for (int i = 0; i < buffer.size(); ++i) {
		if (isspace(buffer[i])) {
			continue;
		}

		directiveName = getDirectiveName(buffer, i);
		i += directiveName.size();

		if (directiveName == KW_SERVER) {
			directive = getBlockDirective(buffer, i);
		} else {
			throw std::exception();
		}
		std::cout << buffer[i];
	}
	return nullptr;
}