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
	String msg = "";
	if (cmd.params.empty())
	{
		Utils::sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "PART"), fd);
		return;
	}
	if (!client->isRegistered())
	{
		Utils::sendReply(Reply::ERR_NOTREGISTERED(nickname), fd);
		return;
	}
	if (!cmd.trailing.empty())
		msg = cmd.trailing;
	std::vector<String> channels = Utils::splitByDelim(cmd.params[0], ',');
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (!ChannelManager::isValidChannelName(channels[i]))
		{
			Utils::sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channels[i]), fd);
			continue;
		}
		channelManager.partChannel(client, channels[i], msg);
	}
}
