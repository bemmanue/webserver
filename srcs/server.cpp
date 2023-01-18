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
}

void server::getSocketDescriptor(const char *port) {
  int status;
  struct addrinfo* record;

  status = getaddrinfo("localhost", port, &hints_, &results);

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

server::server(const char *port)
    : hints_(), socket_(0),
      nfds(1) {
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

  if (port < 1024 && port != 80) {
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

connection *server::getConnection() {
  return connections;
}

sock_t server::getSocket() const {
  return socket_;
}

void server::acceptConnections() {
  sock_t newConnection;
//  socklen_t addrSize;

//  candidate for maybe later
//  newConnection = accept(socket_, (struct sockaddr *)
//                         &theirAddrArr[connections->getNumberOf()], &addrSize);
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
  char buffer[BUFFER_SIZE + 1];
  int rc;

  if (revents & POLLIN) {
    rc = recv(fd, buffer, sizeof (buffer), 0);
    if (rc == 0) {
      std::cout << "Connection closed\n" << std::endl;
    } else if (rc < 0 && errno != EWOULDBLOCK) {
      std::string str = "Connection receive failed\n";
      throw MyException(str);
    } else {
      std::cout << buffer << std::endl;
    }
  } else if (revents & POLLOUT) {
    const char *strrr = "<!doctype html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>Example Domain</title>\n"
        "\n"
        "    <meta charset=\"utf-8\" />\n"
        "    <meta http-equiv=\"Content-type\" content=\"text/html; "
        "charset=utf-8\" />\n"
        "    <meta name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1\" />\n"
        "    <style type=\"text/css\">\n"
        "    body {\n"
        "        background-color: #f0f0f2;\n"
        "        margin: 0;\n"
        "        padding: 0;\n"
        "        font-family: -apple-system, system-ui, BlinkMacSystemFont, "
        "\"Segoe UI\", \"Open Sans\", \"Helvetica Neue\", Helvetica, Arial, "
        "sans-serif;\n"
        "        \n"
        "    }\n"
        "    div {\n"
        "        width: 600px;\n"
        "        margin: 5em auto;\n"
        "        padding: 2em;\n"
        "        background-color: #fdfdff;\n"
        "        border-radius: 0.5em;\n"
        "        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);\n"
        "    }\n"
        "    a:link, a:visited {\n"
        "        color: #38488f;\n"
        "        text-decoration: none;\n"
        "    }\n"
        "    @media (max-width: 700px) {\n"
        "        div {\n"
        "            margin: 0 auto;\n"
        "            width: auto;\n"
        "        }\n"
        "    }\n"
        "    </style>    \n"
        "</head>\n"
        "\n"
        "<body>\n"
        "<div>\n"
        "    <h1>Example Domain</h1>\n"
        "    <p>This domain is for use in illustrative examples in documents. You "
        "may use this\n"
        "    domain in literature without prior coordination or asking for "
        "permission.</p>\n"
        "    <p><a href=\"https://www.iana.org/domains/example\">More "
        "information...</a></p>\n"
        "</div>\n"
        "</body>\n"
        "</html>";
    rc = send(fd, strrr, std::strlen(strrr), 0);
    if (rc < 0) {
      std::string str = "Connection send failed\n";
      throw MyException(str);
    }
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