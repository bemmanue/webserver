#pragma once

#include <netinet/in.h>
#include "utils.hpp"
#include <ctime>

namespace ft
{
	class Connection {
	private:
		struct sockaddr_in	channel_;
		const sock_t		sock_fd_;
		const int 			bind_;
		int 				opts_;
		time_t				ses_;

		Connection operator=(Connection const &rhs);
		Connection(Connection const &rhs);
	public:
		Connection();
		virtual ~Connection();
	};
}