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

  int yes = 1;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));
}

void server::bindSocket() {
  bind_ = bind(socket_, record_->ai_addr, record_->ai_addrlen);

  if (bind_ < 0) {
    std::string str = "bind exception:";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

void server::listenSocket() const {
  if (listen(socket_, FT_LISTEN_CLIENT_LIMIT) < 0) {
    std::string str = "listen exception:";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

server::server(int port)
    : hints_(), socket_(0), record_(NULL),
      bind_(0), opts_(0), ses_(0) {
  initStruct();
  getSocketDescriptor(port);
  bindSocket();
  listenSocket();
}

server::~server() {
}

server *server::getServer() {
  if (instance_ != NULL) {
    instance_ = new server();
  }
  return instance_;
}
const std::list<connection>& server::getConnections() const {
  return connection_lists_;
}
void server::setConnections(const std::list<connection>& connectionLists) {
  connection_lists_ = connectionLists;
}

}  // namespace ft