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
	const String &oldNick = client->getNickname();
	const String &prefix = client->getPrefix();

	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NONICKNAMEGIVEN(oldNick));
		return;
	}
	else if (!client->getAuth())
	{
		client->sendReply(Reply::ERR_PASSWDMISMATCH(oldNick, "required"));
		return;
	}

	String newNick = Utils::strToLower(cmd.params[0]);

	if (!isValidNick(newNick))
	{
		client->sendReply(Reply::ERR_ERRONEUSNICKNAME(oldNick, cmd.params[0]));
		return;
	}
	else if (newNick == oldNick)
		return;
	else if (!isNickAvalible(newNick, clientManager))
	{
		client->sendReply(Reply::ERR_NICKNAMEINUSE(oldNick, cmd.params[0]));
		return;
	}
	else if (!clientManager.changeNick(newNick, client))
		return;

	else if (client->isFirstLogin())
	{
		client->setIsRegistered(true);
		client->sendWelcome(server.getCreationTime());
		return;
	}
	else if (client->isRegistered())
	{
		String reply = Reply::RPL_SUCCNICK(prefix, newNick);
		client->sendReply(reply);
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
