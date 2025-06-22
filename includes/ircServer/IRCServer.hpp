#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <IRC.hpp>
#include <ClientManager.hpp>
#include <EventDispatcher.hpp>
#include <ChannelManager.hpp>

extern bool g_running;

class IRCServer
{
private:
	const int _port;
	const String _password;
	String _creationTime;
	int _serverFd;
	EventDispatcher _eventDispatcher;
	ChannelManager _channelManager;
	ClientManager _clientManager;

public:
	IRCServer(int port, String password);
	~IRCServer() throw();

	void start();
	void stop();
	ClientManager &getClientManager();
	ChannelManager &getChannelManager();
	EventDispatcher &getEventDispatcher();
	const String &getPass();
	const String &getCreationTime();

private:
	void setup();
	void run();
};

#endif
// IRCSERVER_HPP
