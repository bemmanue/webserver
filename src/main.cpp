#include <iostream>

#include "../include/Config.hpp"

int main(int argc, char **argv) {
	std::vector<Config> config;

	if (argc != 2) {
		std::cout << "wrong arguments" << std::endl;
	}

	try {
		config = parseConfig(argv[1]);
		for (int i = 0; i < config.size(); ++i) {
			std::cout << "Config №" << i + 1 << std::endl;
			std::cout << "---------" << std::endl;
			config[i].print();
		}
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}


    return 0;
}
