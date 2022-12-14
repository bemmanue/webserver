#include <iostream>

int main(int argc, char **argv) {
	Config* config;

	if (argc == 2) {
		config = parseConfig(argv[1]);
	} else {
		std::cout << "wrong arguments" << std::endl;
	}

    return 0;
}
