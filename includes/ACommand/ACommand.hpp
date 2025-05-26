#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <IRC.hpp>

class IRCServer;

class ACommand
{
public:
	virtual ~ACommand();
	virtual void execute(int fd, FullCmd &cmd, IRCServer &server) = 0;
};

#endif
