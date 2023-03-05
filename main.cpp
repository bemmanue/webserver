#include "src/http/Response.hpp"
#include "src/config/Config.hpp"
#include "src/core/Client.hpp"

int main(int argc, char **argv) {
	std::vector<ServerConfig> configs;

	try {
		configs = parseConfigFile("conf/webserver.conf");
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
		exit(0);
	}




//	std::ifstream t("request.txt");
//	if (!t.is_open()) {
//		std::cout << "error openning request file" << std::endl;
//		exit(0);
//	}
//
//	t.seekg(0, std::ios::end);
//	size_t size = t.tellg();
//	std::string buffer(size, ' ');
//	t.seekg(0);
//	t.read(&buffer[0], size);
//
//	Client client(config);
//	client.setRequest(buffer);
//	client.handleRequest();

    return 0;
}
