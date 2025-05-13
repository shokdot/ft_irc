#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <EventHandler.hpp>

class IRCServer
{
private:
	int _port;
	String _password;
	int _serverFd;
	EventHandler _eventHandler;

public:
	IRCServer(int port, String password);
	~IRCServer();

private:
	void setup();
	void run();
	struct sockaddr_in createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr);
};

#endif
// IRCSERVER_HPP
