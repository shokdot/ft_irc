#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

// #include <IdentService.hpp>
#include <PollManager.hpp>

class EventDispatcher
{
public:
	void handleEvents();
	void init(int);

private:
	int _serverFd;
	PollManager _pollManager;
	// void handlePOLLIN(int);
	// void handlePOLLERR(int);
	// void handleNewConnection();
	// void handleClientMessage(int clientFd);
};

#endif
// EVENT_DISPATCHER_HPP
