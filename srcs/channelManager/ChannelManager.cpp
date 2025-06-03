#include <ChannelManager.hpp>

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
