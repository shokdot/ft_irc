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
	String nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();
	String msg = "";
	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "PART"));
		return;
	}
	if (!client->isRegistered())
	{
		client->sendReply(Reply::ERR_NOTREGISTERED(nickname));
		return;
	}
	if (!cmd.trailing.empty())
		msg = cmd.trailing;
	std::vector<String> channels = Utils::splitByDelim(cmd.params[0], ',');
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (!ChannelManager::isValidChannelName(channels[i]))
		{
			client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channels[i]));
			continue;
		}
		channelManager.partChannel(client, channels[i], msg);
	}
}
