#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <cstring>

#include "ConfigException.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"

namespace ft {
#define KW_AUTOINDEX "autoindex"
#define KW_CGI_PASS "cgi_pass"
#define KW_CLIENT_MAX_BODY_SIZE "client_max_body_size"
#define KW_ERROR_PAGE "error_page"
#define KW_INDEX "index"
#define KW_HOST "host"
#define KW_LOCATION "location"
#define KW_METHODS_ALLOWED "methods_allowed"
#define KW_PORT "port"
#define KW_REDIRECT "redirect"
#define KW_ROOT "root"
#define KW_SERVER "server"
#define KW_SERVER_NAME "server_name"

#define KW_OPENING_BRACE "{"
#define KW_CLOSING_BRACE "}"
#define KW_SEMICOLON ";"

#define CTX_MAIN 0
#define CTX_SERVER 1
#define CTX_LOCATION 2

#define KB 1000
#define MB 1000 * KB
#define GB 1000 * MB

      class Config {
   public:
    void addServerBlock(const ServerBlock& c);
    bool isEmpty();
    void print();  //debug
    std::vector<ServerBlock> getServers();

   private:
    std::vector<ServerBlock> _servers;
  };

  std::vector<char> readFile(const std::string& filename);
  std::string getNextToken();

  Config parseConfigFile(const std::string& filename);
  ServerBlock getServerBlock();
  LocationBlock getLocationBlock();
}
