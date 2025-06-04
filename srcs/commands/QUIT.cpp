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
	int fd = client->getClientFd();
	if (!cmd.params.empty())
		return;
	if (!cmd.trailing.empty())
	{
		// send braodcast message
	}
	server.getEventDispatcher().disconnectClient(fd, server);
}
