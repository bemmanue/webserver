#pragma once

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <ctime>

#include "utils.hpp"

namespace ft {

class connection {
 public:
  explicit connection(int port);
  ~connection();

 private:
  connection() {};
  connection(connection const&);
  void operator=(connection const&);

  void initStruct();
  void getSocketDescriptor(int const& port);
  void bindSocket();
  void connectSocket();

  struct addrinfo   hints_;
  sock_t            socket_;
  struct addrinfo*  record_;
  int               bind_;
  int               opts_;
  time_t            ses_;

  /*CHECK THAT THIS WORKS CORRECTLY*/

class MyException : public std::exception {
 public:
  explicit MyException(const std::string &err) throw()
      : err_(err) {
  }

  explicit MyException(const char *err) throw()
      : err_(err) {
  }

  MyException(MyException const& ex) throw()
      : err_(ex.err_) {
  }

  virtual ~MyException() throw() {};

  virtual const char *what() const throw() {
    return err_.c_str();
  }

 protected:
  std::string err_;
};

};

}
