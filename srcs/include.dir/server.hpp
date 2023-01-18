#pragma once

#include "connection.hpp"


#include <list>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cerrno>

namespace ft {

class server {
public:
  static server *of(int port);

 private:
  server();
  ~server();
  explicit server(int port);
  server(server const&);
  void operator=(server const&);

  struct addrinfo  hints_;
  struct addrinfo  *record_;
  sock_t           socket_;
  int              bind_;
  int              opts_;
  time_t           ses_;


  void getSocketDescriptor(const int& port);

  std::list<connection> connection_lists_;
  void initStruct();
  void bindSocket();
  void connectSocket();

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

}  // namespace Server
