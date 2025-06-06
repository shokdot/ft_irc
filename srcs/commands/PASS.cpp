#include <PASS.hpp>
#include <IRCServer.hpp>
#include <Replies.hpp>

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

	if (client->getAuth())
	{
		Utils::sendReply(Reply::ERR_ALREADYREGISTRED(client->getNickname()), fd);
		return;
	}
	else if (cmd.params.empty())
	{
		Utils::sendReply(Reply::ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"), fd);
		return;
	}
	else if (cmd.params[0] != server.getPass())
	{
		Utils::sendReply(Reply::ERR_PASSWDMISMATCH(client->getNickname(), "incorrect"), fd);
		server.getEventDispatcher().disconnectClient(fd, server);
		return;
	}

	client->setAuth(true);
	std::cout << "[INFO] Client " << fd << " authorized" << std::endl;
}
