#include <PART.hpp>
#include <IRCServer.hpp>

PART::PART()
{
}

PART::~PART()
{
}

void PART::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	int fd = client->getClientFd();
	String nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();
	if (cmd.params.empty())
	{
		std::string reply = ":localhost 461 pass :Not enough parameters\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	std::vector<String> channels = Utils::splitByDelim(cmd.params[0], ',');
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (!ChannelManager::isValidChannelName(channels[i]))
		{
			std::string reply = ":localhost 403 " + nickname + " " + channels[i] + " :No such channel\r\n";
			Utils::sendWrapper(reply, fd);
			continue;
		}
		channelManager.partChannel(client, channels[i]);
	}
}
