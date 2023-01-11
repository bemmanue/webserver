#include "include.dir/server.hpp"

namespace ft {

server::server() {

}

server::~server(){

}

server *server::getServer()  {
  if (instance_ != NULL) {
    instance_ = new server();
  }
  return instance_;
}

}  // namespace Server