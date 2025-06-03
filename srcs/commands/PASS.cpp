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
	int fd = client->getClientFd();
	if (cmd.params.size() < 1)
	{
		std::string reply = ":localhost 461 pass :Not enough parameters\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	if (cmd.params[0] != server.getPass())
	{
		std::string reply = ":localhost 464 :Password incorrect\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	if (!client)
	{
		std::string reply = "Something went wrong\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else
	{
		if (client->getAuth())
		{
			std::string reply = ":localhost 462 " + client->getNickname() + " :You may not reregister\r\n";
			Utils::sendWrapper(reply, fd);
			return;
		}
		client->setAuth(true);
		std::cout << "[INFO] Client " << fd << " authorized" << std::endl;
	}
}
