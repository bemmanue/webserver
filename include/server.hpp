#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sstream>
#include <cerrno>
#include <cstdio>

#include "connection.hpp"
#include "IOPoll.hpp"
#include "MyException.hpp"
#include "config/ServerBlock.hpp"
#include "../include/Request.hpp"

namespace ft {
static const std::size_t BUFFER_SIZE = 65536;

class server {
 public:
  static server *ofBlock(const ServerBlock& servBlock);

  connection *getConnection();
  sock_t getSocket() const;
  int serve();

 private:
  server();
  explicit server(const ServerBlock& config);
  ~server();
  server(server const&);
  void operator=(server const&);

  /*System calls wrappers*/
  void initStruct();
  void getSocketDescriptor();
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
