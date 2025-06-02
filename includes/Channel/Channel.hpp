#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <IRC.hpp>

class Channel
{
private:
	String name;
	String topic;
	String password;
	std::vector<String> channelUsers;
	std::set<String> operators;
	std::set<String> invitedUsers;
	int userLimit;

	bool isInviteOnly;
	bool isTopicRestricted;

public:
	const String &getName() const;
	const String &getTopic() const;
	const String &getPassword() const;
	const std::vector<String> &getChannelUsers() const;
	const std::set<String> &getOperators() const;
	const std::set<String> &getInvitedUsers() const;
	int getUserLimit() const;
	bool getIsInviteOnly() const;
	bool getIsTopicRestricted() const;

	void setName(const String &newName);
	void setTopic(const String &newTopic);
	void setPassword(const String &newPassword);
	void setUserLimit(int limit);
	void setIsInviteOnly(bool flag);
	void setIsTopicRestricted(bool flag);
};

#endif
// CHANNEL_HPP
