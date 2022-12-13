#pragma once

class server {
public:
    static server &getInstance() {
		static server instance;
		return instance;
    }

private:
	server() {};
	server(server const &);
	void	operator=(server const&);
};

