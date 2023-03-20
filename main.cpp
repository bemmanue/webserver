#include "src/http/Response.hpp"
#include "src/config/Config.hpp"
#include "src/core/Server.hpp"

int main(int argc, char **argv) {
	std::vector<ServerConfig>	configs;
	Server						server;

	try {
		configs = parseConfigFile("conf/webserver.conf");
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	for (auto & config : configs) {
		server.setConfig(config);
	}

	server.start();

    return 0;
}
