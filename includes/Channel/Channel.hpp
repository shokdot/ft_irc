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
	int getChannelSize();
	void setName(const String &newName);
	void setTopic(const String &newTopic);
	void setPassword(const String &newPassword);
	void setUserLimit(int limit);
	void addInviteUser(Client *client);
	void removeInvitedUser(Client *client);
	bool isUserInvited(Client *client);
	bool hasClient(Client *client);
	std::set<Client *> &getChannelUsers();
	bool isClientInvited(Client *client);
	void addMode(char c);
	void removeMode(char c);
	bool hasMode(char c) const;
	bool canJoin(Client *client);
	void removeOperator(Client *client);
	void print();
	bool hasReachedLimit();
	void broadcastToChannel(const String &msg, int senderFd = -1);
	String getUsersList();
	bool isOperator(Client *client);
	String getModeString() const;
	String getModeParams() const;
};

#endif
// CHANNEL_HPP
