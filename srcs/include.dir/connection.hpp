#pragma once

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <ctime>

#include "utils.hpp"

namespace ft {

#define FT_LISTEN_CLIENT_LIMIT 10

class connection {
 public:
  explicit connection(int port);
  ~connection();

 private:
  connection() {};
  connection(connection const&);
  void operator=(connection const&);

};


}  // namespace ft
