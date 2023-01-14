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
//		for (int i = 0; i < config.config.size(); ++i) {
//			std::cout << "Config №" << i + 1 << std::endl;
//			std::cout << "---------" << std::endl;
//			config.config[i].print();
//		}
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}


    return 0;
}
