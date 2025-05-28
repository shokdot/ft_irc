#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <UserManager.hpp>
#include <EventDispatcher.hpp>

class IRCServer
{
private:
	const int _port;
	const String _password;
	int _serverFd;
	bool _running;
	EventDispatcher _eventDispatcher;
	UserManager _userManager;

public:
	IRCServer(int port, String password);
	~IRCServer() throw();

	void start();
	void stop();
	UserManager &getUserManager();
	const String &getPass();

private:
	void setup();
	void run();
};

#endif
// IRCSERVER_HPP
