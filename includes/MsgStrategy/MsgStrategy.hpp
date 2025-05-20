#ifndef MSG_STRATEGY_HPP
#define MSG_STRATEGY_HPP

#include <IEventStrategy.hpp>

class IRCServer;

class MsgStrategy : implements IEventStrategy
{
public:
	MsgStrategy();
	~MsgStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);
};

#endif
// MSG_STRATEGY_HPP
