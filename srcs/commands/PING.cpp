#include <PING.hpp>
#include <IRCServer.hpp>

PING::PING()
{
}

PING::~PING()
{
}

void PING::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	(void)server;

	if (!client)
		return;

	const String &nickname = client->getNickname();

	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NOORIGIN(nickname));
		return;
	}

	client->sendReply(Reply::RPL_PONG(cmd.params[0]));
}
