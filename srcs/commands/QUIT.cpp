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
	if (!client)
		return;
	int fd = client->getClientFd();
	if (client->isRegistered() && !cmd.trailing.empty())
	{
		client->broadcastJoinedChannels(Reply::RPL_QUIT(client->getPrefix(), cmd.trailing));
	}
	else if (client->isRegistered())
	{
		client->broadcastJoinedChannels(Reply::RPL_QUIT(client->getPrefix()));
	}
	server.getEventDispatcher().disconnectClient(fd);
}
