#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <IRC.hpp>

class Client;

class Channel
{
private:
	String _name;
	String _password;
	String _topic;
	std::set<Client *> _channelUsers;
	std::set<Client *> _operators;
	std::set<Client *> _invitedUsers;
	std::set<char> _mode;
	int _userLimit;

public:
	Channel(const String &name, const String &password);

public:
	const String &getName() const;
	const String &getTopic() const;
	const String &getPassword() const;
	void addUser(Client *client);
	bool deleteUser(Client *client);
	int getUserLimit() const;
	bool isInviteOnly() const;
	bool isTopicRestricted() const;
	void addOperator(Client *client);
	int getChannelSize();
	void setName(const String &newName);
	void setTopic(const String &newTopic);
	void setPassword(const String &newPassword);
	void setUserLimit(int limit);
	// void setIsInviteOnly(bool flag);
	// void setIsTopicRestricted(bool flag);
	bool hasClient(Client *client);
	bool isClientInvited(Client *client);
	void addMode(char c);
	void removeMode(char c);
	bool hasMode(char c) const;
	bool canJoin(Client *client);
	void removeOperator(Client *client);
	void print();
	bool hasReachedLimit();
};

#endif
// CHANNEL_HPP
