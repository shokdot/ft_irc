#include <Channel.hpp>
#include <Client.hpp>

Channel::Channel(const String &name, const String &password) : _name(name),
															   _password(password),
															   _topic(""),
															   _userLimit(0)
{
	addMode('t');
	if (!password.empty())
		addMode('k');
}

void Channel::addUser(Client *client)
{
	_channelUsers.insert(client);
	if (_channelUsers.size() == 1)
		addOperator(client);
}

void Channel::addMode(char c)
{
	_mode.insert(c);
}

void Channel::removeMode(char c)
{
	_mode.erase(c);
}

void Channel::addInviteUser(Client *client)
{
	_invitedUsers.insert(client);
}

void Channel::removeInvitedUser(Client *client)
{
	_invitedUsers.erase(client);
}

bool Channel::hasMode(char c) const
{
	return _mode.count(c);
}

bool Channel::deleteUser(Client *client)
{
	_channelUsers.erase(client);
	removeOperator(client);
	removeInvitedUser(client);
	return _channelUsers.empty();
}

void Channel::addOperator(Client *client)
{
	_operators.insert(client);
}

void Channel::removeOperator(Client *client)
{
	_operators.erase(client);
}

const String &Channel::getName() const
{
	return _name;
}

bool Channel::hasClient(Client *client) const
{
	return _channelUsers.count(client);
}

bool Channel::isClientInvited(Client *client) const
{
	return _invitedUsers.count(client);
}

int Channel::getChannelSize() const
{
	return _channelUsers.size();
}

const String &Channel::getTopic() const
{
	return _topic;
}

const String &Channel::getPassword() const
{
	return _password;
}

int Channel::getUserLimit() const
{
	return _userLimit;
}

bool Channel::isInviteOnly() const
{
	return hasMode('i');
}

bool Channel::isTopicRestricted() const
{
	return hasMode('t');
}

void Channel::setName(const String &newName)
{
	_name = newName;
}

void Channel::setTopic(const String &newTopic)
{
	_topic = newTopic;
}

void Channel::setPassword(const String &newPassword)
{
	_password = newPassword;
}

void Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

bool Channel::hasReachedLimit() const
{
	return hasMode('l') && _channelUsers.size() >= _userLimit;
}

bool Channel::canJoin(Client *client) const
{
	return !isInviteOnly() || isClientInvited(client);
}

std::set<Client *> &Channel::getChannelUsers()
{
	return _channelUsers;
}

void Channel::broadcastToChannel(const String &msg, int senderFd)
{
	std::set<Client *> channelUsers = getChannelUsers();
	std::set<Client *>::iterator it = channelUsers.begin();
	for (; it != channelUsers.end(); ++it)
	{
		Client *client = *it;
		if (client->getClientFd() != senderFd)
			client->sendReply(msg);
	}
}

String Channel::getUsersList() const
{
	String nameList;
	for (std::set<Client *>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it)
	{
		Client *user = *it;
		if (_operators.count(user))
			nameList += "@";
		nameList += user->getNickname() + " ";
	}
	return nameList;
}

bool Channel::isOperator(Client *client) const
{
	return _operators.count(client);
}

String Channel::getModeString() const
{
	String modeStr = "+";

	if (hasMode('i'))
		modeStr += "i";
	if (hasMode('t'))
		modeStr += "t";
	if (hasMode('k'))
		modeStr += "k";
	if (hasMode('l'))
		modeStr += "l";

	return modeStr;
}

String Channel::getModeParams() const
{
	String params;

	if (hasMode('k'))
		params += _password + " ";
	if (hasMode('l'))
		params += Utils::toString(_userLimit) + " ";

	if (!params.empty() && params[params.size() - 1] == ' ')
		params.erase(params.size() - 1);

	return params;
}
