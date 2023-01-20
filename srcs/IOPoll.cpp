#include "include.dir/IOPoll.hpp"

namespace ft {

 int IOPoll::pollIn(int fd, connection *connections) {
  int rc;
  char buffer[BUFFER_SIZE + 1];
  std::string str;

  rc = recv(fd, buffer, sizeof (buffer), 0);
  if (rc == 0) {
    std::cout << "Connection closed\n" << std::endl;
    connections->removeConnection(fd);
    return rc;
  }

  while (rc != 0) {
    if (rc < 0) {
      if (errno != EWOULDBLOCK) {
        std::string strerr = "Connection receive failed\n";
        throw MyException(strerr);
      }
      break;
    }
    std::cout << buffer << std::endl;
    str.insert(str.size(), buffer);
    rc = recv(fd, buffer, sizeof(buffer), 0);
  }
  connections->setEventFlag(POLLOUT, fd);
  return rc;
}

int IOPoll::pollOut(int fd, connection *connections) {
  std::stringstream *response;
  std::stringstream response_body;
  response = new std::stringstream();
  response_body << "<title>Test C++ HTTP server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
//                << "<pre>" << stt << "</pre>\n" // testing
                << "<em><small>Test C++ Http server</small></em>\n";
  *response << "HTTP/1.1 200 OK\r\n"
            << "Version: HTTP/1.1\r\n"
            << "Content-Type: text/html; charset=utf-8\r\n"
            << "Content-Length: " << response_body.str().length()
            << "\r\n\r\n"
            << response_body.str();
  int sd;

  sd = send(fd, response_body.str().c_str(), response->str().size(), 0);
  while (sd > 0) {
    if (sd < 0) {
      std::string str = "Connection send failed\n";
      throw MyException(str);
    }
    std::cout << "SENT SIZE: " << sd
              << "\nREAL SIZE: " << response->str().size() << std::endl;
    std::cout << response->str() << std::endl;
    connections->setEventFlag(POLLIN, fd);
    if (sd == response->str().size()) {
      break;
    }
  }
  return sd;
}
}

//https://github.com/bemmanue/webserver/blob/57bab82d5c2241e5bdbedfc14c7968c018140359/srcs/main.cpp
//https://stackoverflow.com/questions/56236188/in-a-tcp-server-client-connection-using-poll-do-i-need-to-set-events-by-han