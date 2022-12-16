#include <iostream>

#include "../include/Config.hpp"

int main(int argc, char **argv) {
	Config* config;

	if (argc != 2) {
		std::cout << "wrong arguments" << std::endl;
	}

	try {
		config = parseConfig(argv[1]);
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}


    return 0;
}
