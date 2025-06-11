#ifndef KICK_HPP
#define KICK_HPP

#include <ACommand.hpp>

class KICK : public ACommand
{
public:
	KICK();
	~KICK();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);

private:
	void handleKick(Client *client, const String &channelName, const String &targetNick, const String &comment, IRCServer &server);
};
#endif
