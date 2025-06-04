#include <JOIN.hpp>
#include <IRCServer.hpp>

JOIN::JOIN() {}

JOIN::~JOIN() {}

void JOIN::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	int fd = client->getClientFd();
	ChannelManager &channelManager = server.getChannelManager();
	if (cmd.params.empty())
	{
		std::string reply = ":localhost 461 JOIN :Not enough parameters\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (!client->isRegistered())
	{
		std::string reply = ":localhost 451 :You have not registered\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	if (cmd.params[0] == "0")
		return; // partAllChannels();
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

		if (!isValidChannelName(channel) || (!key.empty() && !isValidKey(key)))
		{
			std::string reply = ":localhost 403 " + nickname + " " + channel + " :No such channel\r\n";
			Utils::sendWrapper(reply, fd);
			continue;
		}

		channelKeyPairs.push_back(ChannelKey(channel, key));
	}
	return channelKeyPairs;
}

bool JOIN::isValidChannelName(const String &name)
{
	if (name.empty() || name.size() > 50)
		return false;
	else if (name[0] != '#' && name[0] != '&')
		return false;
	else if (name.find(' ') != String::npos || name.find(',') != String::npos || name.find('\a') != String::npos)
		return false;
	return true;
}

bool JOIN::isValidKey(const String &key)
{
	if (key.find(' ') != String::npos || key.find(',') != String::npos)
		return false;
	return true;
}
