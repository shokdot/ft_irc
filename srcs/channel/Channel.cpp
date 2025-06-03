#include <Channel.hpp>

Channel::Channel(const String &name, const String &password) : _name(name),
															   _password(password),
															   _topic(""),
															   _userLimit(0),
															   _isInviteOnly(false),
															   _isTopicRestricted(false)
{
}

void Channel::addUser(const String &nickname)
{
	_channelUsers.insert(nickname);
	if (_channelUsers.size() == 1)
		addOperator(nickname);
}

bool Channel::deleteUser(const String &nickname)
{
	_channelUsers.erase(nickname);
	_operators.erase(nickname);
	_invitedUsers.erase(nickname);
	return _channelUsers.empty();
}

void Channel::addOperator(const String &nickname)
{
	_operators.insert(nickname);
}

const String &Channel::getName() const
{
	return _name;
}

const String &Channel::getTopic() const { return _topic; }

const String &Channel::getPassword() const { return _password; }

int Channel::getUserLimit() const { return _userLimit; }

bool Channel::getIsInviteOnly() const { return _isInviteOnly; }

bool Channel::getIsTopicRestricted() const { return _isTopicRestricted; }

void Channel::setName(const String &newName) { _name = newName; }

void Channel::setTopic(const String &newTopic) { _topic = newTopic; }

void Channel::setPassword(const String &newPassword) { _password = newPassword; }

void Channel::setUserLimit(int limit) { _userLimit = limit; }

void Channel::setIsInviteOnly(bool flag) { _isInviteOnly = flag; }

void Channel::setIsTopicRestricted(bool flag) { _isTopicRestricted = flag; }
