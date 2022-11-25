#pragma once

class Server {

public:
    static Server &getInstance() {
		static Server instance;
		return instance;
    }

private:
	Server() {};
	Server(Server const &);
	void	operator=(Server const&);
};

