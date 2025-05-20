#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <EventDispatcher.hpp>

class IRCServer
{
private:
	int _port;
	String _password;
	int _serverFd;
	bool _running;
	EventDispatcher _eventDispatcher;

public:
	IRCServer(int port, String password);
	~IRCServer() throw();

	void start();
	void stop();

private:
	void setup();
	void run();
	static struct sockaddr_in createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr);
};

#endif
// IRCSERVER_HPP
