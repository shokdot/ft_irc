#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <IRC.hpp>

class EventHandler
{
public:
	EventHandler(int serverFd);
	void handleEvents();

private:
	int _serverFd;
	std::vector<pollfd> _fds;

	void handleNewConnection();
	void handleClientMessage(int clientFd);
};

#endif
