#include <JOIN.hpp>
#include <IRCServer.hpp>

JOIN::JOIN() {}

JOIN::~JOIN() {}

void JOIN::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	String nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();

	if (!client->isRegistered())
	{
		client->sendReply(Reply::ERR_NOTREGISTERED(nickname));
		return;
	}
	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "JOIN"));
		return;
	}
	if (cmd.params[0] == "0")
	{
		channelManager.partAll(client);
		return;
	}
	std::vector<ChannelKey> channelKeyPairs = parseChannels(client, cmd.params);

	for (size_t i = 0; i < channelKeyPairs.size(); ++i)
	{
		const String &channelName = channelKeyPairs[i].channel;
		const String &key = channelKeyPairs[i].key;

		channelManager.joinChannel(client, channelName, key);
	}
}

std::vector<ChannelKey> JOIN::parseChannels(Client *client, std::vector<String> &params)
{
	std::vector<ChannelKey> channelKeyPairs;
	std::vector<String> channels = Utils::splitByDelim(params[0], ',');
	std::vector<String> keys;
	String nickname = client->getNickname();

	if (params.size() > 1)
		keys = Utils::splitByDelim(params[1], ',');

	for (size_t i = 0; i < channels.size(); ++i)
	{
		String channel = channels[i];
		String key = (i < keys.size()) ? keys[i] : "";

		if (!ChannelManager::isValidChannelName(channel) || (!key.empty() && !ChannelManager::isValidKey(key)))
		{
			client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channel));
			continue;
		}

		channelKeyPairs.push_back(ChannelKey(channel, key));
	}
	return channelKeyPairs;
}
