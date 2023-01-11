#include <cstring>
#include <string>

#include "include.dir/connection.hpp"

namespace ft {

void connection::initStruct() {
  bzero(&hints_, sizeof (hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags= AI_PASSIVE;
}

void connection::getSocketDescriptor(int const& port) {
  int status;
  struct addrinfo* record;
  struct addrinfo* results;

  status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints_, &results);

  if (status != 0) {
    throw MyException(std::string(gai_strerror(status)));
    //It may not work - needs to be tested!
  }

  for (record = results; record != NULL; record = record->ai_next) {
    socket_ =
        socket(record->ai_family, record->ai_socktype, record->ai_protocol);
    if (socket_ == -1) {
      continue;
    }
  }
}

connection::connection(int port) {
  initStruct();
  getSocketDescriptor(port);
}

}