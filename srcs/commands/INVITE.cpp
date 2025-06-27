#include <INVITE.hpp>
#include <IRCServer.hpp>

INVITE::INVITE()
{
}

INVITE::~INVITE()
{
}

void INVITE::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	const String &nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();
	ClientManager &clientManager = server.getClientManager();

	if (cmd.params.size() < 2)
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "INVITE"));
		return;
	}
	if (!client->isRegistered())
	{
		client->sendReply(Reply::ERR_NOTREGISTERED(nickname));
		return;
	}

	String inviteeName = cmd.params[0];
	String channelName = cmd.params[1];
	Channel *channel = channelManager.getChannelByName(channelName);
	Client *invitee = clientManager.getClientByNick(inviteeName);

	if (!channel)
	{
		client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channelName));
		return;
	}
	if (!invitee)
	{
		client->sendReply(Reply::ERR_NOSUCHNICK(nickname, inviteeName));
		return;
	}
	if (!channel->hasClient(client))
	{
		client->sendReply(Reply::ERR_NOTONCHANNEL(nickname, channelName));
		return;
	}
	if (channel->isInviteOnly() && !channel->isOperator(client))
	{
		client->sendReply(Reply::ERR_CHANOPRIVSNEEDED(nickname, channelName));
		return;
	}
	if (invitee->isJoinedChannel(channel))
	{
		client->sendReply(Reply::ERR_USERONCHANNEL(nickname, inviteeName, channelName));
		return;
	}
	if (channel->isUserInvited(invitee))
		return;
	channel->addInviteUser(invitee);
	client->sendReply(Reply::RPL_INVITING(nickname, inviteeName, channelName));
	invitee->sendReply(Reply::RPL_SUCCINVITE(client->getPrefix(), inviteeName, channelName));
}
