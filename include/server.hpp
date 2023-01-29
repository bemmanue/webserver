#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sstream>
#include <cerrno>

#include "connection.hpp"
#include "IOPoll.hpp"
#include "MyException.hpp"
#include "config/ServerBlock.hpp"

namespace ft {
static const std::size_t BUFFER_SIZE = 65536;

class server {
 public:
  explicit server(const ServerBlock& config);
  static server *ofPort(const std::string &strPort);

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
  ServerBlock       config_;

  /*Poll*/
  connection* connections;
  nfds_t nfds;
//  struct sockaddr_storage theirAddrArr[FT_LISTEN_CLIENT_LIMIT];

  /*Poll operations with connections*/
  void acceptConnections();
  void sendAndReceive(int fd, short revents);
};

}  // namespace ft
