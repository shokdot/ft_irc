#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <IRC.hpp>

class IRCServer;
class UserManager;
class ChannelManager;

class ACommand
{
public:
	virtual ~ACommand();
	virtual void execute(int fd, CmdStruct &cmd, IRCServer &server) = 0;
};

#endif
