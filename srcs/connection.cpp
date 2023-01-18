#include "include.dir/connection.hpp"

namespace ft {

struct pollfd *connection::getConnections() const {
  return clientFds;
}

bool connection::addConnection(sock_t newFd) {
  if (numberOf + 1 >= FT_LISTEN_CLIENT_LIMIT) {
    return false;
  }
  clientFds[numberOf].fd = newFd;
  clientFds[numberOf].events = POLLIN;
  numberOf++;
  return true;
}

bool connection::removeConnection(sock_t oldFd) {
  int oldLocation;

  for (int i = 0; i < numberOf; i++) {
    if (clientFds[i].fd == oldFd) {
      oldLocation = i;
      bzero(&clientFds[i++], sizeof (struct pollfd));
      if (i == FT_LISTEN_CLIENT_LIMIT) {
        numberOf--;
        return true ;

      }
      std::memcpy(&clientFds[oldLocation], &clientFds[i],
                  sizeof (struct pollfd) * (numberOf - oldLocation));
      bzero(&clientFds[oldLocation], sizeof (struct pollfd));
      numberOf--;
      return true;
    }
  }
  return false;
}

connection::connection(sock_t listeningSocket)
    : clientFds(), numberOf(0)  {
  if (numberOf == 0) {
    clientFds = new struct pollfd[FT_LISTEN_CLIENT_LIMIT];
    bzero(clientFds, sizeof (*clientFds) * 10);
  }
  if (!addConnection(listeningSocket)) {
    std::string str = "poll exception: ";
    str.insert(0, "couldn't init connection");
    throw MyException(str);
  }
}
connection::connection() : numberOf(0), clientFds() {}

}  // namespace ft