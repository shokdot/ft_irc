#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

// #include <IdentService.hpp>
#include <PollManager.hpp>
#include <Acceptor.hpp>
#include <CmdHandler.hpp>

class IRCServer;

class EventDispatcher
{
public:
	EventDispatcher(IRCServer *server);
	void handleEvents();
	void init(int);

private:
	int _serverFd;
	IRCServer *_server;
	PollManager _pollManager;
	Acceptor _acceptor;
	CmdHandler _cmdHandler;
	void handlePollErr(int fd);
	void handlePollIn(int fd);
	void handlePollOut(int fd);
};

#endif
// EVENT_DISPATCHER_HPP
