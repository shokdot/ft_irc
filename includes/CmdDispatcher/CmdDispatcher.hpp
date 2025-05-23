#ifndef CMD_DISPATCHER_HPP
#define CMD_DISPATCHER_HPP

#include <IRC.hpp>
#include <CmdRegistry.hpp>

class IRCServer;

class CmdDispatcher
{
private:
	CmdRegistry _registry;

public:
	void dispatch(int fd, String line, IRCServer &server);
};

#endif
