#include <Channel.hpp>

Channel::Channel(const String &name) : name(name)
{
}

const String &Channel::getName() const { return name; }

const String &Channel::getTopic() const { return topic; }

const String &Channel::getPassword() const { return password; }

const std::vector<String> &Channel::getChannelUsers() const { return channelUsers; }

const std::set<String> &Channel::getOperators() const { return operators; }

const std::set<String> &Channel::getInvitedUsers() const { return invitedUsers; }

int Channel::getUserLimit() const { return userLimit; }

bool Channel::getIsInviteOnly() const { return isInviteOnly; }

bool Channel::getIsTopicRestricted() const { return isTopicRestricted; }

void Channel::setName(const String &newName) { name = newName; }

void Channel::setTopic(const String &newTopic) { topic = newTopic; }

void Channel::setPassword(const String &newPassword) { password = newPassword; }

void Channel::setUserLimit(int limit) { userLimit = limit; }

void Channel::setIsInviteOnly(bool flag) { isInviteOnly = flag; }

void Channel::setIsTopicRestricted(bool flag) { isTopicRestricted = flag; }
