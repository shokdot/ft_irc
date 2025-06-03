#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <IRC.hpp>

class Channel
{
private:
	String _name;
	String _password;
	String _topic;
	std::set<String> _channelUsers;
	std::set<String> _operators;
	std::set<String> _invitedUsers;
	int _userLimit;

	bool _isInviteOnly;
	bool _isTopicRestricted;

public:
	Channel(const String &name, const String &password);

public:
	const String &getName() const;
	const String &getTopic() const;
	const String &getPassword() const;
	void addUser(const String &nickname);
	bool deleteUser(const String &nikcname);
	int getUserLimit() const;
	bool getIsInviteOnly() const;
	bool getIsTopicRestricted() const;
	void addOperator(const String &nickname);

	void setName(const String &newName);
	void setTopic(const String &newTopic);
	void setPassword(const String &newPassword);
	void setUserLimit(int limit);
	void setIsInviteOnly(bool flag);
	void setIsTopicRestricted(bool flag);
};

#endif
// CHANNEL_HPP
