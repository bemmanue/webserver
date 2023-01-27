#include <iostream>

#include "../include/config/Config.hpp"

int main(int argc, char **argv) {
	Config config;

	if (argc != 2) {
		std::cout << "wrong arguments" << std::endl;
		exit(0);
	}

	try {
		config = parseConfigFile(argv[1]);
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
	config.print();

    return 0;
}
