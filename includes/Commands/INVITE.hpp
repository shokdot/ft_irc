#ifndef INVITE_HPP
#define INVITE_HPP

#include <ACommand.hpp>

class INVITE : public ACommand
{
public:
	INVITE();
	~INVITE();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
