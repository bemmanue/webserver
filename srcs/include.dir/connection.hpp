#pragma once

#include <netdb.h>

#include "utils.hpp"
#include <ctime>

namespace ft {

class connection {
 public:
  connection(int port);
  ~connection();

 private:
  connection() {};
  connection(connection const&);
  void operator=(connection const&);

  void initStruct();
  void getSocketDescriptor(int const& port);

  struct addrinfo hints_;
  sock_t          socket_;
  int             bind_;
  int             opts_;
  time_t          ses_;

  /*CHECK THAT THIS WORKS CORRECTLY*/

class MyException : public std::exception {
 public:
  MyException(std::string err) throw() {
    err_ = err;
  }

  ~MyException() throw() {
      delete ret;
  };

  const char *what() const throw() {
    std::string error = std::string("getaddrinfo error:");
    error.append(err_);
    error.append("\n");

    ret = new char[strlen(error.c_str())];
    strncpy(ret, error.c_str(), strlen(error.c_str()));
    return ret;
  }

 private:
  std::string err_;
  static char* ret;
};
};

}
