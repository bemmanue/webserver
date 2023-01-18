#pragma once

#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <ctime>
#include "utils.hpp"
#include "MyException.hpp"
#include <cerrno>

namespace ft {

#define FT_LISTEN_CLIENT_LIMIT 10

class connection {
 public:
  struct pollfd *getConnections() const;
  bool addConnection(sock_t newFd);
  bool removeConnection(sock_t oldFd);

  explicit connection(sock_t listeningSocket);
  connection();

 private:
  int numberOf;
  ~connection() {
    delete[] clientFds;
  }
  void operator=(connection const&);
  struct pollfd *clientFds;
};


}  // namespace ft
