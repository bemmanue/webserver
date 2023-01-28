#pragma once

#include "server.hpp"

namespace ft {
class IOPoll {
 public:
  static int pollIn(int fd, connection *connections, std::string *str);
  static int pollOut(int fd, connection *connections);

 private:
  static int pollOutHelper(int fd, const char *str, size_t *size);
};
}  // namespace ft
