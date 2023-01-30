#pragma clang diagnostic push
#include "../include/server.hpp"

namespace ft {

/*INITIALIZATION OF SERVER*/

static struct addrinfo* results;

void server::initStruct() {
  bzero(&hints_, sizeof (hints_));
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_STREAM;
  hints_.ai_protocol = IPPROTO_TCP;
  hints_.ai_flags = AI_PASSIVE;
}

void server::getSocketDescriptor() {
  int status;
  struct addrinfo* record;
  std::string host = config_.getHost();
  char* port = NULL;

  sprintf(port, "%lu", config_.getPort());
  status = getaddrinfo(host.c_str(), port, &hints_, &results);

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
                 (char *)&opt, sizeof (int)) < 0) {
    std::string str = "setsockopt exception: ";
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

void server::listenSocket() {
  if (listen(socket_, FT_LISTEN_CLIENT_LIMIT) < 0) {
    std::string str = "listen exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  }
  connections = new connection(socket_);
//  bzero(theirAddrArr, sizeof (struct sockaddr_storage) *
//        FT_LISTEN_CLIENT_LIMIT);
}


server::server(const ServerBlock& config)
    : hints_(), socket_(0), record_(),
      config_(config), nfds(1) {
  initStruct();
  getSocketDescriptor();
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

server *server::ofBlock(const ServerBlock& servBlock) {


  server* newServer;
  try {
     newServer = new server(servBlock);
  } catch (MyException &ex) {
    delete newServer;
    throw ex;
  }

  return newServer;
}

/*END OF SERVER INITIALIZATION BLOCK*/

connection *server::getConnection() {
  return connections;
}

sock_t server::getSocket() const {
  return socket_;
}

void server::acceptConnections() {
  sock_t newConnection;

  while (true) {
    newConnection = accept(socket_, NULL, NULL);
    if (newConnection < 0) {
      if (errno != EWOULDBLOCK) {
        throw MyException("accept() failed!");
      }
      break;
    }
    if (!connections->addConnection(newConnection)) {
      std::cout << "Can't accept any more connections!" << std::endl;
      break;
    }
    nfds++;
  }
}

void server::sendAndReceive(int fd, short revents) {
  if (revents & POLLIN) {
    std::string *str = new std::string;
    IOPoll::pollIn(fd, connections, str);
//    Request request = parseRequest(str);
//    Response response(request);
//    std::string resp = response.getResponse();
    delete str;
  }
  if (revents & POLLOUT) {
    IOPoll::pollOut(fd, connections);
  }
}

int server::serve() {
  int pollResult;
  int timeout = 100;
  struct pollfd *fds = connections->getConnections();

  pollResult = poll(fds, nfds, timeout);
  if (pollResult < 0) {
    std::string str = "poll exception: ";
    str.insert(0, strerror(errno));
    throw MyException(str);
  } else if (pollResult == 0) {
    return pollResult;
  }
  for (int i = 0, currentSize = nfds; i < currentSize; i++) {
    if (fds[i].revents == 0) {
      continue ;
    }
    if (fds[i].fd == socket_) {
      acceptConnections();
    } else {
      sendAndReceive(fds[i].fd, fds[i].revents);
    }
  }
  return pollResult;
}

}  // namespace ft
#pragma clang diagnostic pop
