#pragma once

#include "connection.hpp"
#include "MyException.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sstream>
#include <cerrno>

//#define TEST

#ifdef TEST
# define PORT "8081"

#endif

namespace ft {

class server {
 public:
  static server *ofPort(std::string &strPort);

  connection *getConnection();
  sock_t getSocket() const;
  int serve();

 private:
  server();
  explicit server(const char *port);
  ~server();
  server(server const&);
  void operator=(server const&);

  /*System calls wrappers*/
  void initStruct();
  void getSocketDescriptor(const char *port);
  void setOptions() const;
  void bindSocket();
  void listenSocket();

  /*Socket*/
  struct addrinfo   hints_;
  sock_t            socket_;
  struct addrinfo*  record_;

  /*Poll*/
//  struct pollfd fds[FT_LISTEN_CLIENT_LIMIT];
// 10 is maximum allowed connections for server
// moved to connections
  connection * connections;
  static int serverNumber;
  int serverId;
  nfds_t nfds;
//  struct sockaddr_storage theirAddrArr[FT_LISTEN_CLIENT_LIMIT];

  void acceptConnections();
};

}  // namespace ft
