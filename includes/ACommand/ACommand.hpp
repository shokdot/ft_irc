#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <IRC.hpp>
#include <Replies.hpp>

class IRCServer;
class ClientManager;
class Client;

class ACommand
{
public:
	virtual ~ACommand();
	virtual void execute(Client *client, CmdStruct &cmd, IRCServer &server) = 0;
};

#endif
