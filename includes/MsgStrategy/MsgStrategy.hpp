#ifndef MSG_STRATEGY_HPP
#define MSG_STRATEGY_HPP

#include <IEventStrategy.hpp>

class IRCServer;
class UserManager;

class MsgStrategy : implements IEventStrategy
{
private:
	std::unordered_map<int, String> sockBuffer;

public:
	MsgStrategy();
	~MsgStrategy();
	void handleEvent(int fd, PollManager &pollManager, IRCServer &server);

private:
	bool readFromSock(int fd, PollManager &pollManager, UserManager &userManager);
	bool checkBuffLength(int fd, PollManager &PollManager, UserManager &UserManager);
	void processMsg(int fd);
	void disconnect(int fd, int bytes, PollManager &pollManager, UserManager &userManager);
};

#endif
// MSG_STRATEGY_HPP
