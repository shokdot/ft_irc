#include <QUIT.hpp>
#include <IRCServer.hpp>

QUIT::QUIT()
{
}

QUIT::~QUIT()
{
}

void QUIT::execute(int fd, CmdStruct &cmd, IRCServer &server)
{
	if (!cmd.trailing.empty())
		return; // send braodcast message
	server.getEventDispatcher().disconnectClient(fd, server);
}
