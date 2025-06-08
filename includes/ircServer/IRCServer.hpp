#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <ClientManager.hpp>
#include <EventDispatcher.hpp>
#include <ChannelManager.hpp>

class IRCServer
{
private:
	const int _port;
	const String _password;
	String _creationTime;
	int _serverFd;
	bool _running;
	EventDispatcher _eventDispatcher;
	ChannelManager _channelManager;
	ClientManager _clientManager;

public:
	IRCServer(int port, String password);
	~IRCServer() throw();

	void start();
	void stop();
	ClientManager &getClientManager();
	EventDispatcher &getEventDispatcher();
	ChannelManager &getChannelManager();
	const String &getPass();
	const String &getCreationTime();

private:
	void setup();
	void run();
};

#endif
// IRCSERVER_HPP
