#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <IRC.hpp>

class Channel
{
	String name;
	String topic;
	String password;
	std::vector<String> channelUsers;
	std::set<String> operators;
	std::set<String> invitedUsers;
	int userLimit;

	bool isInviteOnly;
	bool isTopicRestricted;
};

#endif
// CHANNEL_HPP
