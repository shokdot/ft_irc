#ifndef CHANNEL_MANAGER_HPP
#define CHANNEL_MANAGER_HPP

#include <IRC.hpp>
#include <Channel.hpp>

class ChannelManager
{
private:
	std::map<String, Channel *> _channelsByName;

public:
	~ChannelManager();
	Channel *getChannelByName(const String &name);
	Channel *getOrCreateChannel(const String &name);

	void deleteChannel(const String &name);
	// join channel
	// part channel
	// is invited
	// void broadcastToChannel
};

#endif
