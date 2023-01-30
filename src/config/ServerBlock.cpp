#include "../../include/config/ServerBlock.hpp"

namespace ft {
ServerBlock::ServerBlock()
    : _host(DEFAULT_HOST),
      _port(DEFAULT_PORT),
      _client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE) {}

ServerBlock::~ServerBlock() {}

bool isHostAddr(const std::string& addr) {
  in_addr_t in_addr;
  struct in_addr ip;
  hostent* hostnames;

  in_addr = inet_addr(addr.c_str());
  if (in_addr == INADDR_NONE) {
    return false;
  }

  ip.s_addr = in_addr;

  hostnames = gethostbyaddr(&ip, sizeof(ip), AF_INET);
  if (hostnames == NULL || hostnames[0].h_name == NULL) {
    return false;
  }
  return true;
}

bool isHostName(const std::string& addr) {
  if (!gethostbyname(addr.c_str())) {
    return false;
  }
  return true;
}

void ServerBlock::setHost(const std::string& addr) {
  if (isHostAddr(addr) || isHostName(addr)) {
    _host = addr;
  } else {
    throw std::exception();
  }
}

void ServerBlock::setPort(size_t port) {
  if (port > 65535) {
    throw std::exception();
  }
  _port = port;
}

void ServerBlock::setServerName(const std::string& name) {
  _server_names.insert(name);
}

void ServerBlock::setErrorPages(int code, const std::string& path) {
  if (code < 300 || code > 599) {
    throw std::exception();
  }
  _error_pages[code] = path;
}

void ServerBlock::setClientMaxBodySize(uint64_t bytes) {
  _client_max_body_size = bytes;
}

void ServerBlock::setLocation(const std::string& path,
                              const LocationBlock& location) {
  _locations[path] = location;
}

void ServerBlock::print() {
  std::cout << "\t"
            << "host: " << _host << std::endl;
  std::cout << "\t"
            << "port: " << _port << std::endl;

  for (std::set<std::string>::iterator i = _server_names.begin();
       i != _server_names.end(); i++) {
    std::cout << "\t"
              << "server_name: " << *i << std::endl;
  }

  for (std::map<int, std::string>::iterator i = _error_pages.begin();
       i != _error_pages.end(); i++) {
    std::cout << "\t"
              << "error_page: " << (*i).first << " " << (*i).second
              << std::endl;
  }

  std::cout << "\t"
            << "client_max_body_size: " << _client_max_body_size << std::endl;

  for (std::map<std::string, LocationBlock>::iterator i = _locations.begin();
       i != _locations.end(); i++) {
    std::cout << "\t"
              << "Location: " << (*i).first << std::endl;
    (*i).second.print();
  }
}

std::string ServerBlock::getHost() const {
  return _host;
}

size_t ServerBlock::getPort() const {
  return _port;
}

std::set<std::string> ServerBlock::getServerNames() const {
  return _server_names;
}

std::map<int, std::string> ServerBlock::getErrorPages() const {
  return _error_pages;
}

uint64_t ServerBlock::getClientMaxBodySize() const {
  return _client_max_body_size;
}

std::map<std::string, LocationBlock> ServerBlock::getLocation() const {
  return _locations;
}
}  // namespace ft
