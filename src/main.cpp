#include "../include/server.hpp"
#include "../include/config/Config.hpp"

int main(int argc, char* argv[]) {
  Config config;

  if (argc != 2) {
    std::cout << "wrong arguments" << std::endl;
    exit(0);
  }

  try {
    config = parseConfigFile(argv[1]);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
    exit(1);
  }

  std::list<ft::server*> serverList;
  std::vector<ServerBlock> serverConfigs;
  serverConfigs = config.getServers();
  for (int i = 0; i < serverConfigs.size(); i++) {
    serverList.push_back(new ft::server(serverConfigs[i]));
  }

  serverList.push_back(ft::server::ofPort(std::string("8081")));
  while (true) {
    for (std::list<ft::server*>::iterator iter = serverList.begin();
         iter != serverList.end(); ++iter) {
      (*iter)->serve();
    }
  }
}