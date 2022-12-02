#pragma once

#include <netinet/in.h>
#include "utils.hpp"
#include <ctime>

namespace ft
{
	class Connection {
	private:
		struct sockaddr_in	channel_;
		sock_t				socket_;
		const int 			bind_;
		int 				opts_;
		time_t				ses_;

	};
}