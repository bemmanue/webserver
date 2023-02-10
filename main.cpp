#include "http/Response.hpp"
#include "config/Config.hpp"

int main(int argc, char **argv) {
	Config config;

	try {
		config = parseConfigFile("webserver.conf");
	} catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
		exit(0);
	}

	std::ifstream t("request.txt");
	if (!t.is_open()) {
		std::cout << "error openning request file" << std::endl;
		exit(0);
	}
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);

	Request request(buffer);
	ServerBlock serverConfig = config.getServers()[0];
	std::cout << request << std::endl;

	Response response(serverConfig, request);
	std::cout << response.toString();

    return 0;
}
