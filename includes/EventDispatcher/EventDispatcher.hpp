#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include <PollManager.hpp>
#include <MsgStrategy.hpp>
#include <ErrorStrategy.hpp>
#include <AcceptStrategy.hpp>

class IRCServer;

class EventDispatcher
{
public:
	EventDispatcher(IRCServer &server);
	~EventDispatcher();
	void init(int fd);
	void handleEvents();
	void disconnectServer();
	void disconnectClient(int fd);
	void disconnectAllClients();

private:
	int _serverFd;
	IRCServer &_server;
	PollManager _pollManager;
	std::map<int, IEventStrategy *> _strategies;
};

#endif
// EVENT_DISPATCHER_HPP
