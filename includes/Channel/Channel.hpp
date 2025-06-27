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
	size_t _userLimit;

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
	int getChannelSize() const;
	void setName(const String &newName);
	void setTopic(const String &newTopic);
	void setPassword(const String &newPassword);
	void setUserLimit(int limit);
	void addInviteUser(Client *client);
	void removeInvitedUser(Client *client);
	bool isUserInvited(Client *client) const;
	bool hasClient(Client *client) const;
	std::set<Client *> &getChannelUsers();
	bool isClientInvited(Client *client) const;
	void addMode(char c);
	void removeMode(char c);
	bool hasMode(char c) const;
	bool canJoin(Client *client) const;
	void removeOperator(Client *client);
	bool hasReachedLimit() const;
	void broadcastToChannel(const String &msg, int senderFd = -1);
	String getUsersList() const;
	bool isOperator(Client *client) const;
	String getModeString() const;
	String getModeParams() const;
};

#endif
// CHANNEL_HPP
