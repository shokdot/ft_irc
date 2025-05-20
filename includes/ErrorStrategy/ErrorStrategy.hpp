#ifndef ERROR_STRATEGY_HPP
#define ERROR_STRATEGY_HPP

#include <IEventStrategy.hpp>

class ErrorStrategy : implements IEventStrategy
{
public:
	ErrorStrategy();
	~ErrorStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);
};

#endif
