#include "Server.hpp"

Server::Server() {

}

Server::Server(const Server &other) {
	operator=(other);
}

Server &Server::operator=(const Server &other) {
	if (this != &other) {

	}
	return *this;
}

Server::~Server() = default;

void Server::setConfig(const ServerConfig &config) {

}

void Server::start() {

}

#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <ctime>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <cstring>

#define TRUE             1
#define FALSE            0

int createSocket(int port) {
	struct sockaddr_in6   addr;
	int	rc;
	int	on = 1;

	int listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sd < 0) {
		perror("socket() failed");
		exit(-1);
	}

	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0) {
		perror("setsockopt() failed");
		close(listen_sd);
		exit(-1);
	}

	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	rc = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (rc < 0) {
		perror("ioctl() failed");
		close(listen_sd);
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family      = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port        = htons(port);
	rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
		perror("bind() failed");
		close(listen_sd);
		exit(-1);
	}

	rc = listen(listen_sd, 32);
	if (rc < 0) {
		perror("listen() failed");
		close(listen_sd);
		exit(-1);
	}
	return listen_sd;
}

//int main (int argc, char *argv[]) {
//	int		len, rc;
//	int		listen_sd = -1, listen_sd2 = -1, new_sd = -1;
//	int		end_server = FALSE, compress_array = FALSE;
//	int		close_conn;
//	char	buffer[1000];
//	//	struct	sockaddr_in6   addr;
//	int		timeout;
//	struct	pollfd fds[200];
//	int		nfds = 2, current_size = 0, i, j;
//	int		listen_fd;
//
//	memset(fds, 0 , sizeof(fds));
//
//	listen_sd = createSocket(12345);
//	fds[0].fd = listen_sd;
//	fds[0].events = POLLIN;
//
//	listen_sd2 = createSocket(12346);
//	fds[1].fd = listen_sd2;
//	fds[1].events = POLLIN;
//
//	timeout = (3 * 60 * 1000);
//
//	do {
//		rc = poll(fds, nfds, timeout);
//		if (rc < 0) {
//			perror("poll() failed");
//			break;
//		}
//
//		if (rc == 0) {
//			printf("poll() timed out. End program.\n");
//			break;
//		}
//
//		current_size = nfds;
//		for (i = 0; i < current_size; i++) {
//
//			if (fds[i].revents == 0)
//				continue;
//
//			if (fds[i].revents != POLLIN) {
//				//				printf("Error! revents = %d\n", fds[i].revents);
//				continue;
//			}
//
//			if (fds[i].fd == listen_sd || fds[i].fd == listen_sd2) {
//				listen_fd = fds[i].fd;
//				printf("Listening socket is readable %d\n", listen_fd);
//
//				do {
//					new_sd = accept(listen_fd, NULL, NULL);
//					if (new_sd < 0) {
//						if (errno != EWOULDBLOCK) {
//							perror("accept() failed");
//							end_server = TRUE;
//						}
//						break;
//					}
//
//					printf("New incoming connection - %d\n", new_sd);
//					fds[nfds].fd = new_sd;
//					fds[nfds].events = POLLIN;
//					nfds++;
//
//				} while (new_sd != -1);
//
//			} else {
//				printf("Descriptor %d is readable\n", fds[i].fd);
//				close_conn = FALSE;
//
//				do {
//					/*****************************************************/
//					/* Receive data on this connection until the         */
//					/* recv fails with EWOULDBLOCK. If any other         */
//					/* failure occurs, we will close the                 */
//					/* connection.                                       */
//					/*****************************************************/
//					rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
//					if (rc < 0) {
//						if (errno != EWOULDBLOCK) {
//							perror("recv() failed");
//							close_conn = TRUE;
//						}
//						break;
//					}
//
//					if (rc == 0) {
//						printf("Connection closed\n");
//						close_conn = TRUE;
//						break;
//					}
//
//					len = rc;
//					printf("%d bytes received\n", len);
//
//				} while(TRUE);
//
//				rc = send(fds[i].fd, "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: 20\r\n\r\nhello from webserver", 91, 0);
//				if (rc < 0) {
//					perror("send() failed");
//					close_conn = TRUE;
//					break;
//				}
//
//				/*******************************************************/
//				/* If the close_conn flag was turned on, we need       */
//				/* to clean up this active connection. This            */
//				/* clean up process includes removing the              */
//				/* descriptor.                                         */
//				/*******************************************************/
//				if (close_conn) {
//					close(fds[i].fd);
//					fds[i].fd = -1;
//					compress_array = TRUE;
//				}
//
//
//			}  /* End of existing connection is readable             */
//		} /* End of loop through pollable descriptors              */
//
//		/***********************************************************/
//		/* If the compress_array flag was turned on, we need       */
//		/* to squeeze together the array and decrement the number  */
//		/* of file descriptors. We do not need to move back the    */
//		/* events and revents fields because the events will always*/
//		/* be POLLIN in this case, and revents is output.          */
//		/***********************************************************/
//		if (compress_array) {
//			compress_array = FALSE;
//			for (i = 0; i < nfds; i++) {
//				if (fds[i].fd == -1) {
//					for (j = i; j < nfds; j++) {
//						fds[j].fd = fds[j+1].fd;
//					}
//					i--;
//					nfds--;
//				}
//			}
//		}
//
//	} while (end_server == FALSE); /* End of serving running.    */
//
//	/*************************************************************/
//	/* Clean up all of the sockets that are open                 */
//	/*************************************************************/
//	for (i = 0; i < nfds; i++) {
//		if (fds[i].fd >= 0) {
//			close(fds[i].fd);
//		}
//	}
//}


