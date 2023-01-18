#include "include.dir/server.hpp"

int main(int argc, char *argv[]) {
  socklen_t sinSize;
  int newFd;
  struct sockaddr_storage their_addr;
  std::list<ft::server *> serverList;

  std::string testPort = "8081";
  serverList.push_back(ft::server::ofPort(testPort));
  while (true) {
    for (std::list<ft::server *>::iterator iter = serverList.begin();
         iter != serverList.end();
         ++iter) {
      (*iter)->serve();
    }
  }
}
