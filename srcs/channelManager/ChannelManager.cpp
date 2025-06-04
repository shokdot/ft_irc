#include <ChannelManager.hpp>
#include <Client.hpp>

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

void ChannelManager::joinChannel(Client *client, const String &name, const String &password)
{
	int fd = client->getClientFd();
	Channel *channel = getOrCreateChannel(name, password);
	if (!channel)
	{
		std::string reply = ":localhost 475 " + client->getNickname() + " " + name + " :Cannot join channel (+k)\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (channel->hasClient(client))
		return;
	else if (!channel->canJoin(client))
	{
		std::string reply = ":localhost 473 " + client->getNickname() + " " + name + " :Cannot join channel (+i)\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (channel->hasReachedLimit())
	{
		std::string reply = ":localhost 471 " + client->getNickname() + " " + name + " :Cannot join channel (+l)\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	channel->addUser(client);
	client->joinChannel(channel);
	channel->print(); // comment this
					  // bradcast msg and other thing else
}

void ChannelManager::partChannel(Client *client, const String &name)
{
	Channel *channel = getChannelByName(name);
	if (!channel)
		return;
	if (!client->isJoinedChannel(channel))
		return;
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
		partChannel(client, name);
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
