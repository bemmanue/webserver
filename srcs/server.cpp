#include "include.dir/server.hpp"

namespace ft {

/*INITIALIZATION OF SERVER*/

static struct addrinfo* results;

void server::initStruct() {
  bzero(&hints_, sizeof (hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags= AI_PASSIVE;
  bzero(fds, sizeof (struct pollfd) * 10);
}

void server::getSocketDescriptor(const char *port) {
  int status;
  struct addrinfo* record;

  status = getaddrinfo(NULL, port, &hints_, &results);

  if (status != 0) {
    std::string str = "getaddrinfo exception: ";
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
    std::string str = "setsockopt exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }

  if (ioctl(socket_, FIONBIO, &opt) < 0) {
    std::string str = "ioctl exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

void server::bindSocket() {
  int aBind = bind(socket_, record_->ai_addr, record_->ai_addrlen);

  if (aBind < 0) {
    std::string str = "bind exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }

  if (record_ == NULL) {
    std::string str = "server:failed to bind exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

void server::listenSocket() const {
  if (listen(socket_, FT_LISTEN_CLIENT_LIMIT) < 0) {
    std::string str = "listen exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
}

server::server(const char *port)
    : hints_(), socket_(0), record_(NULL), fds(), nfds(1) {
  serverId = serverNumber++;
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

/*END OF SERVER INITIALIZATION BLOCK*/

connection server::getConnection() const {
  return connection_list_;
}

sock_t server::getSocket() const {
  return socket_;
}

int server::serve() {
  int pollResult;
  int timeout = 100;


  pollResult = poll(fds, nfds, timeout);
  if (pollResult < 0) {
    std::string str = "poll exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  } else if (pollResult == 0) {
    return pollResult;
  }
  for (int i = 0, currentSize = nfds; i < currentSize; i++) {

  }

  return pollResult;
}

}  // namespace ft