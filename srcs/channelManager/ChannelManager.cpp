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
	channel->print(); // comment this
					  // bradcast msg and other thing else
}
