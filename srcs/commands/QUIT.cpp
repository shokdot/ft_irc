#include <QUIT.hpp>
#include <IRCServer.hpp>

QUIT::QUIT()
{
}

QUIT::~QUIT()
{
}

void QUIT::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	(void)server;
	(void)client;
	(void)cmd;
	// if (!cmd.trailing.empty())
	// 	return; // send braodcast message
	// server.getEventDispatcher().disconnectClient(fd, server);
}
