#ifndef CMD_DISPATCHER_HPP
#define CMD_DISPATCHER_HPP

#include <IRC.hpp>
#include <CmdRegistry.hpp>
#include <CmdParser.hpp>

class Client;
class IRCServer;

class CmdDispatcher
{
private:
	CmdRegistry _registry;

public:
	void dispatch(Client *client, String line, IRCServer &server);
};

#endif
