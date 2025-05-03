#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>

class IRCServer
{
private:
	int port;
	String password;
	int server_fd;

public:
	IRCServer(std::pair<int, String>);
	~IRCServer();

private:
	void setup();
	void run();
};

#endif
// IRCSERVER_HPP
