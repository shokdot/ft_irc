#include <USER.hpp>
#include <IRCServer.hpp>

USER::USER()
{
}

USER::~USER()
{
}

void USER::execute(int fd, CmdStruct &cmd, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	Client *client = clientManager.getClientByFd(fd);
	if (!client)
	{
		std::string reply = ":localhost 451 :You have not registered\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (client->isRegistered())
	{
		std::string reply = ":localhost 462 :" + client->getNickname() + " already registered\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (cmd.params.size() < 3 || cmd.trailing.empty())
	{
		std::string reply = ":localhost 461 USER :Not enough parameters\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (!isValidUsername(cmd.params[0]))
	{
		std::string reply = ":localhost 468 USER :Invalid username\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	client->setRealname(cmd.trailing);
	client->setUsername(cmd.params[0]);
	std::cout << "[INFO] Client " << fd << " registered" << std::endl;
}

bool USER::isValidUsername(const String &username)
{
	if (username.empty())
		return false;
	std::string invalidChars = " @\n\r\0";
	for (size_t i = 0; i < username.size(); ++i)
	{
		if (invalidChars.find(username[i]) != std::string::npos)
			return false;
	}
	return true;
}
