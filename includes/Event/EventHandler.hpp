#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <IRC.hpp>

class EventHandler
{
public:
	EventHandler(int);
	void handleEvents();

private:
	int serverFd;
	std::vector<pollfd> fds;

	struct pollfd createConnection(int, short);
	void handlePOLLIN(int);
	void handlePOLLERR(int);
	void handleNewConnection();
	void handleClientMessage(int clientFd);
};

#endif
// EVENT_HANDLER_HPP
