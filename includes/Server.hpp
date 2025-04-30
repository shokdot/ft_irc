#ifndef SERVER_HPP
#define SERVER_HPP

#include <IRC.hpp>

class Server
{
private:
	int port;
	String password;
	int server_fd;

public:
	Server(std::pair<int, String>);
	~Server();

private:
	void setup();
	void run();
};

#endif
// SERVER_HPP
