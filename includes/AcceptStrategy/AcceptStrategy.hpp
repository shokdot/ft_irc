#ifndef ACCEPT_STRATEGY_HPP
#define ACCEPT_STRATEGY_HPP

#include <IEventStrategy.hpp>

class AcceptStrategy : public IEventStrategy
{
public:
	AcceptStrategy();
	~AcceptStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);
};

#endif
// ACCEPT_STRATEGY_HPP
