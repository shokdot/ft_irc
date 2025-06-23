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
	Channel *getOrCreateChannel(const String &name, const String &password);
	void deleteChannel(const String &name);

	void joinChannel(Client *client, const String &name, const String &password);
	void partChannel(Client *client, const String &name, const String &msg);
	void partChannel(Client *client, const String &name);
	void partAll(Client *clinet);

	void sendJoinRPL(Client *client, Channel *channel);

	static bool isValidChannelName(const String &name);
	static bool isValidKey(const String &name);
};

#endif
// CHANNEL_MANAGER_HPP
