#include <PASS.hpp>
#include <IRCServer.hpp>

PASS::PASS()
{
}

PASS::~PASS()
{
}

void PASS::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	int fd = client->getClientFd();
	const String &nickname = client->getNickname();

	if (client->getAuth())
	{
		client->sendReply(Reply::ERR_ALREADYREGISTRED(nickname));
		return;
	}
	else if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "PASS"));
		return;
	}
	else if (cmd.params[0] != server.getPass())
	{
		client->sendReply(Reply::ERR_PASSWDMISMATCH(nickname, "incorrect"));
		// server.getEventDispatcher().disconnectClient(fd);
		return;
	}

	client->setAuth(true);
	std::cout << "[INFO] Client " << fd << " authorized" << std::endl;
}
