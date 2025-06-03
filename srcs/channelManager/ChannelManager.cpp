#include <ChannelManager.hpp>

ChannelManager::~ChannelManager()
{
	std::map<String, Channel *>::iterator it = _channelsByName.begin();
	for (; it != _channelsByName.end(); ++it)
	{
		delete it->second;
	}
	_channelsByName.clear();
}

Channel *ChannelManager::getChannelByName(const String &name)
{
	std::map<String, Channel *>::iterator it = _channelsByName.find(name);
	if (it != _channelsByName.end())
		return _channelsByName[name];
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

Channel *ChannelManager::getOrCreateChannel(const String &name)
{
	if (_channelsByName.count(name) == 0)
	{
		Channel *channel = new Channel(name);
		_channelsByName[name] = channel;
	}
	return _channelsByName[name];
}
