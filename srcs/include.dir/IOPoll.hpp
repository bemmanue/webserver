#pragma once

#include "server.hpp"

namespace ft {
class IOPoll {
 public:
  static int pollIn(int fd, connection *connections);
  static int pollOut(int fd, connection *connections);
};
}  // namespace ft
