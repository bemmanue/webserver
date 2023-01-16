#pragma once

#include "connection.hpp"
#include <list>
#include <iostream>
#include <sstream>

//#define TEST

#ifdef TEST
# define PORT "8081"

#endif

namespace ft {

class server {
 public:
  static server *ofPort(std::string &strPort);

  const std::list<connection>& getConnections() const;
  void setConnections(const std::list<connection>& connectionLists);
  sock_t getSocket() const;


 private:
  server();
  explicit server(const char *port);
  ~server();
  server(server const&);
  void operator=(server const&);

  /*System calls wrappers*/
  void initStruct();
  void getSocketDescriptor(const char *port);
  void setOptions() const;
  void bindSocket();
  void listenSocket() const;

  /*Socket */
  struct addrinfo   hints_;

 private:
  sock_t            socket_;
  struct addrinfo*  record_;
  int               bind_;

  std::list<connection> connection_lists_;

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

}  // namespace ft
