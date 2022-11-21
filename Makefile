all:		webserv

webserv:
			c++ -std=c++98 -pedantic *.cpp -o webserv

clean:
			rm -rf webserv
