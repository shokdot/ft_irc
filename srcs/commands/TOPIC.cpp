#include <TOPIC.hpp>
#include <IRCServer.hpp>

TOPIC::TOPIC() {}

TOPIC::~TOPIC() {}

void TOPIC::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	String nickname = client->getNickname();

	if (!client->isRegistered())
	{
		client->sendReply(Reply::ERR_NOTREGISTERED(nickname));
		return;
	}
	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "TOPIC"));
		return;
	}

	String channelName = cmd.params[0];
	ChannelManager &channelManager = server.getChannelManager();
	Channel *channel = channelManager.getChannelByName(channelName);

	if (!channel)
	{
		client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channelName));
		return;
	}
	if (!channel->hasClient(client))
	{
		client->sendReply(Reply::ERR_NOTONCHANNEL(nickname, channelName));
		return;
	}
	if (cmd.trailing.empty())
	{
		if (channel->getTopic().empty())
		{
			client->sendReply(Reply::RPL_NOTOPIC(nickname, channelName));
			return;
		}
		else
		{
			client->sendReply(Reply::RPL_TOPIC(nickname, channelName, channel->getTopic()));
			return;
		}
	}
	else
	{
		if (channel->isTopicRestricted() && !channel->isOperator(client))
		{
			client->sendReply(Reply::ERR_CHANOPRIVSNEEDED(nickname, channelName));
			return;
		}
		cmd.trailing = cmd.trailing.substr(1);
		channel->setTopic(cmd.trailing);
		channel->broadcastToChannel(Reply::RPL_SUCCTOPIC(client->getPrefix(), channelName, cmd.trailing));
	}
}
