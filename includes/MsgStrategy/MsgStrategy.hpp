#ifndef MSG_STRATEGY_HPP
#define MSG_STRATEGY_HPP

#include <IEventStrategy.hpp>
#include <CmdDispatcher.hpp>

class IRCServer;
class EventDispatcher;
class ClientManager;

class MsgStrategy : implements IEventStrategy
{
private:
	CmdDispatcher _dispatcher;
	std::map<int, String> sockBuffer;

public:
	MsgStrategy();
	~MsgStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);

private:
	bool readFromSock(int fd, IRCServer &server);
	bool checkBuffLength(int fd, IRCServer &server);
	void processMsg(int fd, IRCServer &server);
	void disconnect(int fd, int bytes, IRCServer &server);
};

#endif
// MSG_STRATEGY_HPP
