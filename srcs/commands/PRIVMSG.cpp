#include <PRIVMSG.hpp>
#include <IRCServer.hpp>

PRIVMSG::PRIVMSG()
{
}

PRIVMSG::~PRIVMSG()
{
}

void PRIVMSG::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	String nickname = client->getNickname();

	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NORECIPIENT(nickname, "PRIVMSG"));
		return;
	}
	if (cmd.trailing.empty())
	{
		client->sendReply(Reply::ERR_NOTEXTTOSEND(nickname));
		return;
	}
	std::vector<String> targets = Utils::splitByDelim(cmd.params[0], ',');
	if (targets.size() > 20)
	{
		client->sendReply(Reply::ERR_TOOMANYTARGETS(nickname, cmd.params[0]));
		return;
	}
	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (!handleTarget(client, targets[i], cmd.trailing, server))
			continue;
	}
}

bool PRIVMSG::handleTarget(Client *client, const String &target, const String &msg, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	ChannelManager &channelManager = server.getChannelManager();
	String nickname = client->getNickname();

	if (ChannelManager::isValidChannelName(target))
	{
		Channel *channel = channelManager.getChannelByName(target);
		if (!channel)
		{
			client->sendReply(Reply::ERR_NOSUCHNICK(nickname, target));
			return false;
		}
		if (!channel->hasClient(client))
		{
			client->sendReply(Reply::ERR_CANNOTSENDTOCHAN(nickname, target));
			return false;
		}
		channel->broadcastToChannel(Reply::RPL_PRIVMSG(client->getPrefix(), target, msg), client->getClientFd());
	}
	else
	{
		Client *targetClient = clientManager.getClientByNick(target);
		if (!targetClient)
		{
			client->sendReply(Reply::ERR_NOSUCHNICK(nickname, target));
			return false;
		}
		targetClient->sendReply(Reply::RPL_PRIVMSG(client->getPrefix(), target, msg));
	}
	return true;
}
