#ifndef IEVENT_STRATEGY_HPP
#define IEVENT_STRATEGY_HPP

#include <IRC.hpp>
#include <PollManager.hpp>

class IRCServer;

class IEventStrategy
{
public:
	virtual ~IEventStrategy();
	virtual void handleEvent(int fd, PollManager &pollManager, IRCServer &server) = 0;
};

#endif
