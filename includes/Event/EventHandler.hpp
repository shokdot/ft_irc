#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <IRC.hpp>

class EventHandler
{
public:
	EventHandler();
	void handleEvents();
	void setServerFd(int);
	int getServerFd();

private:
	int _serverFd;
	std::vector<pollfd> _fds;

	struct pollfd createConnection(int, short);
	void handlePOLLIN(int);
	void handlePOLLERR(int);
	void handleNewConnection();
	void handleClientMessage(int clientFd);
};

#endif
// EVENT_HANDLER_HPP
