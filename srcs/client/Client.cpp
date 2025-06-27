#include <Client.hpp>
#include <Replies.hpp>

Client::Client(int socketFd, struct sockaddr_in clientAddr) : _socketFd(socketFd),
															  _clientAddr(clientAddr),
															  _username("*"),
															  _nickname("*"),
															  _realname("unknown"),
															  _hostname("unknown"),
															  _isAuth(false),
															  _isRegistered(false) {}

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

void Client::setHostname()
{
	char *ip = inet_ntoa(_clientAddr.sin_addr);
	_hostname = String(ip);
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
	return _isRegistered;
}

void Client::setIsRegistered(bool flag)
{
	_isRegistered = flag;
}

void Client::joinChannel(Channel *channel)
{
	_joinedChannels.insert(channel);
}

void Client::removeFromChannel(Channel *channel)
{
	_joinedChannels.erase(channel);
}

bool Client::isJoinedChannel(Channel *channel) const
{
	return _joinedChannels.count(channel);
}

std::set<Channel *> &Client::getJoinedChannels()
{
	return _joinedChannels;
}

void Client::broadcastJoinedChannels(const String &msg) const
{
	std::set<Channel *>::iterator it = _joinedChannels.begin();

	for (; it != _joinedChannels.end(); ++it)
	{
		(*it)->broadcastToChannel(msg, _socketFd);
	}
}

String Client::getPrefix() const
{
	String prefix;
	if (isRegistered())
		prefix = _nickname + "!" + _username + "@" + _hostname;
	else
		prefix = _nickname;
	return prefix;
}

bool Client::hasNick() const
{
	return _nickname != "*";
}

bool Client::hasUser() const
{
	return _username != "*";
}

bool Client::isFirstLogin() const
{
	return (!isRegistered() && hasNick() && hasUser());
}

void Client::sendWelcome(const String &time) const
{
	sendReply(Reply::RPL_WELCOME(_nickname, getPrefix()));
	sendReply(Reply::RPL_YOURHOST(_nickname));
	sendReply(Reply::RPL_CREATED(_nickname, time));
	sendReply(Reply::RPL_MYINFO(_nickname));
}

void Client::sendReply(const String &str) const
{
	String msg = str + "\r\n";
	send(_socketFd, msg.c_str(), msg.length(), 0);
}
