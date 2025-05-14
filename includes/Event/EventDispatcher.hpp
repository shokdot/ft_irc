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
	void handlePollErr(int fd);
	void handlePollIn(int fd);
	void handlePollOut(int fd);
};

#endif
// EVENT_DISPATCHER_HPP
