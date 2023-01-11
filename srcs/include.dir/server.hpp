#pragma once

#include "connection.hpp"
#include <list>

namespace ft {

class server {
 public:
  static server *getServer();

 private:
  server();
  ~server();
  server(server const&);
  void operator=(server const&);

  static server *instance_;
  std::list<connection> connection_lists_;
};

}  // namespace Server
