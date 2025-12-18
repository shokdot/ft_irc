#ifndef MSG_STRATEGY_HPP
#define MSG_STRATEGY_HPP

#include <IEventStrategy.hpp>
#include <CmdDispatcher.hpp>

class IRCServer;
class EventDispatcher;
class ClientManager;

class MsgStrategy : public IEventStrategy
{
private:
	CmdDispatcher _dispatcher;
	std::map<int, String> _sockBuffer;

public:
	MsgStrategy();
	~MsgStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);

private:
	void processMsg(int fd, IRCServer &server);
	bool readFromSock(int fd, EventDispatcher &eventDispatcher);
	bool checkBuffLength(int fd, EventDispatcher &eventDispatcher);
	void disconnect(int fd, int bytes, EventDispatcher &eventDispatcher);
};

#endif
// MSG_STRATEGY_HPP
