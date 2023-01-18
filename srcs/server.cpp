#include "include.dir/server.hpp"

namespace ft {

void server::initStruct() {
  bzero(&hints_, sizeof (hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags= AI_PASSIVE;
}

void server::getSocketDescriptor(int const& port) {
  int status;
  struct addrinfo* record;
  struct addrinfo* results;

  status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints_, &results);

  if (status != 0) {
    std::string str = "getaddrinfo exception:";
    str.insert(0, gai_strerror(status));
    throw MyException(str);
  }

  for (record = results; record != NULL; record = record->ai_next) {
    socket_ = socket(
        record->ai_family,
        record->ai_socktype,
        record->ai_protocol);
    if (socket_ == -1) {
      continue;
    } else {
      record_ = record;
      break;
    }
  }
}

void server::bindSocket() {
  bind_ = bind(socket_, record_->ai_addr, record_->ai_addrlen);

  if (bind_ < 0) {
    std::string str = "bind exception:";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

void server::connectSocket() {

}

server::server(int port)
    : hints_(), record_(), socket_(),
      bind_(), opts_(), ses_() {
  initStruct();
  getSocketDescriptor(port);
  bindSocket();
  connectSocket();
}
server * server::of(int port) {
  return new server(port);
}

server::~server() {}

server::server() {}

}  // namespace Server