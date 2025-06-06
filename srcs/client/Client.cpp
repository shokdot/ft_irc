#include <Client.hpp>

Client::Client(int socketFd) : _socketFd(socketFd),
							   _username("*"),
							   _nickname("*"),
							   _realname("unknown"),
							   _hostname("unknown"),
							   _isAuth(false) {}

Client::~Client() {}

void Client::setUsername(const String &username)
{
	this->_username = username;
}

void Client::setNickname(const String &nickname)
{
	this->_nickname = nickname;
}

void Client::setRealname(const String &realname)
{
	this->_realname = realname;
}

void Client::setHostname(const String &hostname)
{
	this->_hostname = hostname;
}

void Client::setAuth(bool auth)
{
	this->_isAuth = auth;
}

const String &Client::getUsername() const
{
	return this->_username;
}

const String &Client::getNickname() const
{
	return this->_nickname;
}

const String &Client::getRealname() const
{
	return this->_realname;
}

const String &Client::getHostname() const
{
	return this->_hostname;
}

int Client::getClientFd() const
{
	return this->_socketFd;
}

bool Client::getAuth() const
{
	return this->_isAuth;
}

bool Client::isRegistered() const
{
	if (_isAuth && _nickname != "*" && _username != "*")
		return true;
	return false;
}

void Client::joinChannel(Channel *channel)
{
	_joinedChannels.insert(channel);
}

void Client::removeChannel(Channel *channel)
{
	_joinedChannels.erase(channel);
}

bool Client::isJoinedChannel(Channel *channel)
{
	return _joinedChannels.count(channel);
}

std::set<Channel *> &Client::getJoinedChannels()
{
	return _joinedChannels;
}

void Client::broadcastJoinedChannels(const String &msg)
{
	std::set<Channel *>::iterator it = _joinedChannels.begin();

	for (; it != _joinedChannels.end(); ++it)
	{
		(*it)->broadcastToChannel(msg, _socketFd);
	}
}

String Client::getPrefix() const
{
	String prefix = _nickname + "!" + _username + "@" + _hostname;
	return prefix;
}
