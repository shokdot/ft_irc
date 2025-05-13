#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

// #include <IdentService.hpp>

class EventDispatcher
{
public:
	EventDispatcher();
	// void handleEvents();
	void setServerFd(int);

private:
	int _serverFd;
	// void handlePOLLIN(int);
	// void handlePOLLERR(int);
	// void handleNewConnection();
	// void handleClientMessage(int clientFd);
};

#endif
// EVENT_DISPATCHER_HPP
