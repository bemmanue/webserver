#include "src/http/Response.hpp"
#include "src/config/Config.hpp"

int main(int argc, char **argv) {
	std::vector<ServerConfig> config;

	try {
		config = parseConfigFile("conf/webserver.conf");
		for (int i = 0; i < config.size(); ++i) {
			std::cout << config[i];
		}
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

	ServerConfig serverConfig = config[0];
	Request request(serverConfig, buffer);
	std::cout << request << std::endl;

	Response response(serverConfig, request);
	std::cout << response.toString();

    return 0;
}
