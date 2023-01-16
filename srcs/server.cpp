#include "include.dir/server.hpp"

namespace ft {

static struct addrinfo* results;

void server::initStruct() {
  bzero(&hints_, sizeof (hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags= AI_PASSIVE;
}

void server::getSocketDescriptor(const char *port) {
  int status;
  struct addrinfo* record;

  status = getaddrinfo(NULL, port, &hints_, &results);

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

void server::setOptions() const {
  int opt = 1;

  if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
                 &opt, sizeof (int)) < -1) {
    std::string str = "setsockopt exception:";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

void server::bindSocket() {
  bind_ = bind(socket_, record_->ai_addr, record_->ai_addrlen);

  if (bind_ < 0) {
    std::string str = "bind exception:";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }

  if (record_ == NULL) {
    std::string str = "server:failed to bind exception:";
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

server::server(const char *port)
    : hints_(), socket_(0), record_(NULL),
      bind_(0) {
  initStruct();
  getSocketDescriptor(port);
  setOptions();
  bindSocket();
  listenSocket();
  freeaddrinfo(results);
  results = NULL;
}

server::server() {
}

server::~server() {
}

server *server::ofPort(std::string &strPort) {

  std::stringstream ss(strPort);
  short port;
  ss >> port;

  if (port < 1024) {
    throw MyException("Trying to init server on restricted port");
  }

  server* newServer;
  try {
     newServer = new server(strPort.c_str());
  } catch (MyException &ex) {
    delete newServer;
    throw ex;
  }
  return newServer;
}

const std::list<connection>& server::getConnections() const {
  return connection_lists_;
}
void server::setConnections(const std::list<connection>& connectionLists) {
  connection_lists_ = connectionLists;
}
sock_t server::getSocket() const {
  return socket_;
}

}  // namespace ft