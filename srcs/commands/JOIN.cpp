#include <JOIN.hpp>
#include <IRCServer.hpp>

JOIN::JOIN() {}

JOIN::~JOIN() {}

void JOIN::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	int fd = client->getClientFd();
	String nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();

	if (cmd.params.empty())
	{
		Utils::sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "JOIN"), fd);
		return;
	}
	else if (!client->isRegistered())
	{
		Utils::sendReply(Reply::ERR_NOTREGISTERED(nickname), fd);
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
	int fd = client->getClientFd();
	String nickname = client->getNickname();

	if (params.size() > 1)
		keys = Utils::splitByDelim(params[1], ',');

	for (size_t i = 0; i < channels.size(); ++i)
	{
		String channel = channels[i];
		String key = (i < keys.size()) ? keys[i] : "";

		if (!ChannelManager::isValidChannelName(channel) || (!key.empty() && !ChannelManager::isValidKey(key)))
		{
			Utils::sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channel), fd);
			continue;
		}

		channelKeyPairs.push_back(ChannelKey(channel, key));
	}
	return channelKeyPairs;
}
