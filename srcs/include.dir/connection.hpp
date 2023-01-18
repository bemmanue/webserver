#pragma once

#include <netinet/in.h>
#include "utils.hpp"
#include <ctime>

namespace ft
{
	class connection {
	private:
		connection operator=(connection const &rhs);
		connection(connection const &rhs);

	public:
		connection();
		virtual ~connection();
    };
}