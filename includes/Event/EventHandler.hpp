#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

// #include <IdentService.hpp>

class EventHandler
{
public:
	EventHandler();
	// void handleEvents();
	void setServerFd(int);
	int getServerFd();

private:
	int _serverFd;
	// void handlePOLLIN(int);
	// void handlePOLLERR(int);
	// void handleNewConnection();
	// void handleClientMessage(int clientFd);
};

#endif
// EVENT_HANDLER_HPP
