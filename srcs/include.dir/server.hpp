#pragma once

#include "connection.hpp"
#include <list>

namespace ft {

class server {
 public:
  static server *getServer();

  const std::list<connection>& getConnections() const;
  void setConnections(const std::list<connection>& connectionLists);

 private:
  server();
  explicit server(int port);
  ~server();
  server(server const&);
  void operator=(server const&);

  /*System calls wrappers*/
  void initStruct();
  void getSocketDescriptor(int const& port);
  void bindSocket();
  void listenSocket() const;

  struct addrinfo   hints_;
  sock_t            socket_;
  struct addrinfo*  record_;
  int               bind_;
  int               opts_;
  time_t            ses_;

  static server *instance_;
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
