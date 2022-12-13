/*
#include <iostream>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 8000
#define BUF_SIZE 8192
#define QUEUE_SIZE 10

int main() {
    int sock, b, l, sa, on = 1;
	size_t res;
    char buf[BUF_SIZE];
    struct sockaddr_in channel;
	std::stringstream response;
	std::stringstream response_body;

    bzero(&channel, sizeof (channel));
    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);
    channel.sin_port = htons(SERVER_PORT);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) exit(1);

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof (on));
    b = bind(sock, (struct sockaddr *) &channel, sizeof (channel));
    if (b < 0) exit(1);

    l = listen(sock, QUEUE_SIZE);
    if (l < 0) exit(1);

    while (true) {
        sa = accept(sock, nullptr, nullptr);
        if (sa < 0) {
			std::cerr << "accept failed: " << std::strerror(errno) << std::endl;
			exit(1);
		}

        res = recv(sa, buf, BUF_SIZE, 0);
		if (res == SO_ERROR) {
			std::cerr << "Recv failed: " << res << std::endl;
			close(sock);
		} else if (res == 0) {
			std::cerr << "Connection closed" << std::endl;
		} else if (res > 0) {
			buf[res] = '\0';

			response_body << "<title>Test C++ HTTP server</title>\n"
						  << "<h1>Test page</h1>\n"
						  << "<p>This is body of the test page...</p>\n"
						  << "<h2>Request headers</h2>\n"
						  << "<pre>" << buf << "</pre>\n"
						  << "<em><small>Test C++ Http server</small></em>\n";
			response << "HTTP/1.1 200 OK\r\n"
					 << "Version: HTTP/1.1\r\n"
					 << "Content-Type: text/html; charset=utf-8\r\n"
					 << "Content-Length: " << response_body.str().length()
					 << "\r\n\r\n"
					 << response_body.str();

			res = send(sa, response.str().c_str(),
					   response.str().length(), 0);

			if (res == SO_ERROR) {
				std::cerr << "Send failed: " << std::strerror(errno) <<
				std::endl;
			}
			close(sa);
		}
	}
    return 0;
}
*/
