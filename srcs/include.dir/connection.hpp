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

enum status {
  RECEIVE = false,
  SEND = true
};

class connection {
 public:
  struct pollfd *getConnections();
  bool addConnection(sock_t newFd);
  bool removeConnection(sock_t oldFd);
  void setSendable(status flag, sock_t responceSocket);
//  int getNumberOf() const;

  explicit connection(sock_t listeningSocket);
  connection(connection &rhs);
  connection &operator=(connection const &rhs);
  connection();
  ~connection() {
    delete[] clientFds;
  }

 private:
  int numberOf;
  bool events[FT_LISTEN_CLIENT_LIMIT];
  struct pollfd *clientFds;
};


}  // namespace ft
