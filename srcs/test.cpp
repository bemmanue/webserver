#define SERVER_PORT "8000"
#define BUF_SIZE 8192

#include <cstring>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>

int main() {
	const size_t buffer_size = BUF_SIZE;
	int status;
	struct addrinfo hints;
	struct addrinfo *record;
	struct addrinfo *results;
	int server_socket;

	bzero(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	//null, as this is the server program, and we have no ip address
	if ((status = getaddrinfo(NULL, SERVER_PORT, &hints, &results)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
	}

	for (record = results; record != NULL; record = record->ai_next) {
      server_socket = socket(
          record->ai_family, record->ai_socktype,
          record->ai_protocol);
      if (server_socket == -1) {
          continue ;
      }
      //once the socket is created we configure it (so that the listening
      // socket's port can be reused
//		connect( )
	}
    freeaddrinfo(results);
}