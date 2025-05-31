#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <ClientManager.hpp>
#include <EventDispatcher.hpp>

class IRCServer
{
private:
	const int _port;
	const String _password;
	int _serverFd;
	bool _running;
	EventDispatcher _eventDispatcher;
	ClientManager _clientManager;

public:
	IRCServer(int port, String password);
	~IRCServer() throw();

	void start();
	void stop();
	ClientManager &getClientManager();
	EventDispatcher &getEventDispatcher();
	const String &getPass();

private:
	void setup();
	void run();
};

#endif
// IRCSERVER_HPP
