#include "../include/server.hpp"
#include "../include/config/Config.hpp"

int main(int argc, char* argv[]) {
  ft::Config config;

  if (argc != 2) {
    std::cout << "wrong arguments" << std::endl;
    exit(0);
  }

  try {
    config = ft::parseConfigFile(argv[1]);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
    exit(1);
  }

  std::list<ft::server*> serverList;
  std::vector<ft::ServerBlock> serverConfigs;
  serverConfigs = config.getServers();

  serverList.push_back(ft::server::ofBlock(serverConfigs.front()));
  while (true) {
    for (std::list<ft::server*>::iterator iter = serverList.begin();
         iter != serverList.end(); ++iter) {
      (*iter)->serve();
    }
  }
}