#ifndef CMD_HANDLER_HPP
#define CMD_HANDLER_HPP

#include <IRC.hpp>

class IRCServer;

class CmdHandler
{
private:
	IRCServer *_server;

public:
	void setServer(IRCServer *server);
	void execute();
};

#endif
