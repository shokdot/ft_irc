#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include <ACommand.hpp>

class PRIVMSG : public ACommand
{
public:
	PRIVMSG();
	~PRIVMSG();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);

private:
	bool handleTarget(Client *client, const String &channelName, const String &msg, IRCServer &server);
};

#endif
