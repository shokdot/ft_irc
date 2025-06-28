#include <USER.hpp>
#include <IRCServer.hpp>

USER::USER()
{
}

USER::~USER()
{
}

void USER::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	(void)server;

	if (!client)
		return;

	int fd = client->getClientFd();
	const String &nickname = client->getNickname();

	if (cmd.params.size() == 4 && cmd.trailing.empty())
	{
		cmd.trailing = cmd.params.back();
		cmd.params.pop_back();
	}
	if (cmd.params.size() < 3 || cmd.trailing.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "USER"));
		return;
	}
	if (!client->getAuth())
	{
		client->sendReply(Reply::ERR_PASSWDMISMATCH(nickname, "required"));
		return;
	}
	if (client->isRegistered())
	{
		client->sendReply(Reply::ERR_ALREADYREGISTRED(nickname));
		return;
	}
	if (!isValidUsername(cmd.params[0]))
	{
		client->sendReply(Reply::ERR_INVALIDUSERNAME(cmd.params[0]));
		return;
	}

	client->setRealname(cmd.trailing);
	client->setUsername(cmd.params[0]);
	client->setHostname();
	std::cout << "[INFO] Client " << fd << " registered" << std::endl;

	if (client->isFirstLogin())
	{
		client->setIsRegistered(true);
		client->sendWelcome(server.getCreationTime());
	}
}

bool USER::isValidUsername(const String &username)
{
	if (username.empty())
		return false;
	const String invalidChars = " *@\n\r\0";
	for (size_t i = 0; i < username.size(); ++i)
	{
		if (invalidChars.find(username[i]) != std::string::npos)
			return false;
	}
	return true;
}
