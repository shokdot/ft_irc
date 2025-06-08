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

	int fd = client->getClientFd();
	String nickname = client->getNickname();
	String channelName = channel->getName();

	if (channel->getTopic().empty())
	{
		Utils::sendReply(Reply::RPL_NOTOPIC(nickname, channelName), fd);
	}
	else
	{
		Utils::sendReply(Reply::RPL_TOPIC(nickname, channelName, channel->getTopic()), fd);
	}
}

void ChannelManager::joinChannel(Client *client, const String &name, const String &password)
{
	if (!client)
		return;

	int fd = client->getClientFd();
	String nickname = client->getNickname();
	Channel *channel = getOrCreateChannel(name, password);

	if (!channel)
	{
		Utils::sendReply(Reply::ERR_BADCHANNELKEY(nickname, name), fd);
		return;
	}
	if (channel->hasClient(client))
		return;
	if (!channel->canJoin(client))
	{
		Utils::sendReply(Reply::ERR_INVITEONLYCHAN(nickname, name), fd);
		return;
	}
	if (channel->hasReachedLimit())
	{
		Utils::sendReply(Reply::ERR_CHANNELISFULL(nickname, name), fd);
		return;
	}

	channel->addUser(client);
	client->joinChannel(channel);
	channel->broadcastToChannel(Reply::RPL_JOIN(client->getPrefix(), name), -1);
	sendJoinRPL(client, channel);
}

void ChannelManager::partChannel(Client *client, const String &name, const String &msg) // fix replys
{
	Channel *channel = getChannelByName(name);
	if (!channel)
		return;
	if (!client->isJoinedChannel(channel))
		return;
	if (!client->isQuitting())
	{
		if (!msg.empty())
			channel->broadcastToChannel(msg, client->getClientFd());
		String reply = "PART " + name + msg;
		channel->broadcastToChannel(reply, client->getClientFd());
	}

	client->removeChannel(channel);
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
		partChannel(client, name, "");
	}
}

bool ChannelManager::isValidChannelName(const String &name)
{
	if (name.empty() || name.size() > 50)
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
