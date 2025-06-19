#include <KICK.hpp>
#include <IRCServer.hpp>

KICK::KICK() {}

KICK::~KICK() {}

void KICK::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	String nickname = client->getNickname();

	if (cmd.params.size() < 2)
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "KICK"));
		return;
	}

	std::vector<String> channelNames = Utils::splitByDelim(cmd.params[0], ',');
	std::vector<String> userNames = Utils::splitByDelim(cmd.params[1], ',');

	bool oneChannel = (channelNames.size() == 1);
	if (!oneChannel && channelNames.size() != userNames.size())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "KICK"));
		return;
	}

	if (oneChannel && !server.getChannelManager().getChannelByName(channelNames[0]))
	{
		client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channelNames[0]));
		return;
	}

	String msg = cmd.trailing.empty() ? "" : cmd.trailing;

	for (size_t i = 0; i < userNames.size(); ++i)
	{
		String channelName = oneChannel ? channelNames[0] : channelNames[i];
		String targetNick = userNames[i];
		handleKick(client, channelName, targetNick, msg, server);
	}
}

void KICK::handleKick(Client *client, const String &channelName, const String &targetNick, const String &msg, IRCServer &server)
{
	String nickname = client->getNickname();

	ChannelManager &channelManager = server.getChannelManager();
	ClientManager &clientManager = server.getClientManager();

	Channel *channel = channelManager.getChannelByName(channelName);
	Client *targetClient = clientManager.getClientByNick(targetNick);

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

	if (!targetClient)
	{
		client->sendReply(Reply::ERR_NOSUCHNICK(nickname, targetNick));
		return;
	}

	if (!channel->isOperator(client))
	{
		client->sendReply(Reply::ERR_CHANOPRIVSNEEDED(nickname, channelName));
		return;
	}

	if (!channel->hasClient(targetClient))
	{
		client->sendReply(Reply::ERR_USERNOTINCHANNEL(nickname, channelName));
		return;
	}

	channel->broadcastToChannel(Reply::RPL_SUCCKICK(client->getPrefix(), channelName, targetNick, msg));

	targetClient->removeFromChannel(channel);

	if (channel->deleteUser(targetClient))
		channelManager.deleteChannel(channelName);
}
