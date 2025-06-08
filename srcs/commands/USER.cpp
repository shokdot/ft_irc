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
	if (!client)
		return;

	int fd = client->getClientFd();
	String nickname = client->getNickname();
	if (cmd.params.size() == 4 && cmd.trailing.empty())
	{
		cmd.trailing = cmd.params.back();
		cmd.params.pop_back();
	}
	if (cmd.params.size() < 3 || cmd.trailing.empty())
	{
		Utils::sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "USER"), fd);
		return;
	}
	if (!client->getAuth())
	{
		Utils::sendReply(Reply::ERR_PASSWDMISMATCH(nickname, "required"), fd);
		return;
	}
	if (client->isRegistered())
	{
		Utils::sendReply(Reply::ERR_ALREADYREGISTRED(nickname), fd);
		return;
	}
	if (!isValidUsername(cmd.params[0]))
	{
		Utils::sendReply(Reply::ERR_INVALIDUSERNAME(cmd.params[0]), fd);
		return;
	}

	client->setRealname(cmd.trailing);
	client->setUsername(cmd.params[0]);
	std::cout << "[INFO] Client " << fd << " registered" << std::endl;

	if (client->isFirstLogin())
	{
		client->setIsRegistered(true);
		client->sendWelcome();
	}
}

bool USER::isValidUsername(const String &username)
{
	if (username.empty())
		return false;
	std::string invalidChars = " *@\n\r\0";
	for (size_t i = 0; i < username.size(); ++i)
	{
		if (invalidChars.find(username[i]) != std::string::npos)
			return false;
	}
	return true;
}
