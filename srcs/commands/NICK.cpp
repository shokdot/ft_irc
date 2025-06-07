#include <NICK.hpp>
#include <IRCServer.hpp>

NICK::NICK()
{
}

NICK::~NICK()
{
}

void NICK::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	ClientManager &clientManager = server.getClientManager();
	int fd = client->getClientFd();
	String oldNick = client->getNickname();
	String prefix = client->getPrefix();

	if (cmd.params.empty())
	{
		Utils::sendReply(Reply::ERR_NONICKNAMEGIVEN(oldNick), fd);
		return;
	}
	else if (!client->getAuth())
	{
		Utils::sendReply(Reply::ERR_PASSWDMISMATCH(oldNick, "required"), fd);
		return;
	}

	String newNick = Utils::strToLower(cmd.params[0]);

	if (!isValidNick(newNick))
	{
		Utils::sendReply(Reply::ERR_ERRONEUSNICKNAME(oldNick, cmd.params[0]), fd);
		return;
	}
	else if (newNick == oldNick)
		return;
	else if (!isNickAvalible(newNick, clientManager))
	{
		Utils::sendReply(Reply::ERR_NICKNAMEINUSE(oldNick, cmd.params[0]), fd);
		return;
	}
	else if (!clientManager.changeNick(newNick, client)) // check, fix
		return;

	else if (client->isFirstLogin())
	{
		client->setIsRegistered(true);
		client->sendWelcome();
		return;
	}
	else if (client->isRegistered())
	{
		String reply = Reply::RPL_SUCCNICK(prefix, newNick);
		Utils::sendReply(reply, fd);
		client->broadcastJoinedChannels(reply);
	}
}

bool NICK::isValidNick(const String &nickname)
{
	if (nickname.empty() || nickname.size() > 9)
		return false;
	if (!(std::isalpha(static_cast<unsigned char>(nickname[0])) || isSpecial(static_cast<unsigned char>(nickname[0]))))
		return false;
	char c;
	for (size_t i = 0; i < nickname.size(); ++i)
	{
		c = static_cast<unsigned char>(nickname[i]);
		if (!(std::isalnum(c) || isSpecial(c) || c == '-'))
			return false;
	}
	return true;
}

bool NICK::isSpecial(char c)
{
	return (c >= '[' && c <= '`') || (c >= '{' && c <= '}');
}

bool NICK::isNickAvalible(const String &nickname, ClientManager &clientManager)
{
	if (clientManager.getClientByNick(nickname))
		return false;
	return true;
}
