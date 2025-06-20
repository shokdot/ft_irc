#include <ChannelManager.hpp>
#include <Client.hpp>
#include <Replies.hpp>

ChannelManager::~ChannelManager()
{
	std::map<String, Channel *>::iterator it = _channelsByName.begin();
	for (; it != _channelsByName.end(); ++it)
		delete it->second;
	_channelsByName.clear();
}

Channel *ChannelManager::getChannelByName(const String &name)
{
	std::map<String, Channel *>::iterator it = _channelsByName.find(name);
	if (it != _channelsByName.end())
		return it->second;
	return NULL;
}

void ChannelManager::deleteChannel(const String &name)
{
	std::map<String, Channel *>::iterator it = _channelsByName.find(name);
	if (it != _channelsByName.end())
	{
		delete it->second;
		_channelsByName.erase(it);
	}
}

Channel *ChannelManager::getOrCreateChannel(const String &name, const String &password)
{
	std::map<String, Channel *>::iterator it = _channelsByName.find(name);

	if (it == _channelsByName.end())
	{
		Channel *channel = new Channel(name, password);
		_channelsByName[name] = channel;
		return channel;
	}
	if (it->second->getPassword() != password)
		return NULL;
	return it->second;
}

void ChannelManager::sendJoinRPL(Client *client, Channel *channel)
{

	if (!channel || !client)
		return;

	String nickname = client->getNickname();
	String channelName = channel->getName();

	if (channel->getTopic().empty())
		client->sendReply(Reply::RPL_NOTOPIC(nickname, channelName));
	else
		client->sendReply(Reply::RPL_TOPIC(nickname, channelName, channel->getTopic()));

	client->sendReply(Reply::RPL_NAMREPLY(nickname, channelName, channel->getUsersList()));
	client->sendReply(Reply::RPL_ENDOFNAMES(nickname, channelName));
}

void ChannelManager::joinChannel(Client *client, const String &name, const String &password)
{
	if (!client)
		return;

	String nickname = client->getNickname();
	Channel *channel = getOrCreateChannel(name, password);

	if (!channel)
	{
		client->sendReply(Reply::ERR_BADCHANNELKEY(nickname, name));
		return;
	}
	if (channel->hasClient(client))
		return;
	if (!channel->canJoin(client))
	{
		client->sendReply(Reply::ERR_INVITEONLYCHAN(nickname, name));
		return;
	}
	if (channel->hasReachedLimit())
	{
		client->sendReply(Reply::ERR_CHANNELISFULL(nickname, name));
		return;
	}

	channel->addUser(client);
	client->joinChannel(channel);
	channel->removeInvitedUser(client);
	channel->broadcastToChannel(Reply::RPL_JOIN(client->getPrefix(), name));
	sendJoinRPL(client, channel);
}

void ChannelManager::partChannel(Client *client, const String &name, const String &msg)
{
	Channel *channel = getChannelByName(name);
	if (!client || !channel)
		return;

	if (!client->isJoinedChannel(channel))
	{
		client->sendReply(Reply::ERR_NOTONCHANNEL(client->getNickname(), name));
		return;
	}

	if (!client->isQuitting())
		channel->broadcastToChannel(Reply::RPL_PART(client->getPrefix(), name, msg));

	client->removeFromChannel(channel);
	if (channel->deleteUser(client))
		deleteChannel(name);
}

void ChannelManager::partAll(Client *client)
{
	std::set<Channel *> channels = client->getJoinedChannels();
	std::set<Channel *>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
	{
		String name = (*it)->getName();
		partChannel(client, name);
	}
}

bool ChannelManager::isValidChannelName(const String &name)
{
	if (name.size() < 2 || name.size() > 50)
		return false;
	else if (name[0] != '#' && name[0] != '&')
		return false;
	else if (name.find(' ') != String::npos || name.find(',') != String::npos || name.find('\a') != String::npos)
		return false;
	return true;
}

bool ChannelManager::isValidKey(const String &key)
{
	if (key.find(' ') != String::npos || key.find(',') != String::npos)
		return false;
	return true;
}
