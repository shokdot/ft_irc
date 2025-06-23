#include <CAP.hpp>
#include <IRCServer.hpp>

CAP::CAP()
{
}

CAP::~CAP()
{
}

void CAP::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	(void)server;
	if (!cmd.params.empty() && cmd.params[0] == "LS")
		client->sendReply(Reply::RPL_CAP());
}
