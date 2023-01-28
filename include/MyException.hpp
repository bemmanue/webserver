#pragma once

#include <exception>
#include <string>

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