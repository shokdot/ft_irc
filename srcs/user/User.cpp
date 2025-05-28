#include <User.hpp>

User::User(int socketFd) : _socketFd(socketFd),
						   _username("unknown"),
						   _nickname("*"),
						   _realname("unknown"),
						   _hostname("unknown"),
						   _isAuth(false) {}

User::~User() {}

void User::setUsername(const String &username)
{
	this->_username = username;
}

void User::setNickname(const String &nickname)
{
	this->_nickname = nickname;
}

void User::setRealname(const String &realname)
{
	this->_realname = realname;
}

void User::setHostname(const String &hostname)
{
	this->_hostname = hostname;
}

void User::setAuth(bool auth)
{
	this->_isAuth = auth;
}

const String &User::getUsername() const
{
	return this->_username;
}

const String &User::getNickname() const
{
	return this->_nickname;
}

const String &User::getRealname() const
{
	return this->_realname;
}

const String &User::getHostname() const
{
	return this->_hostname;
}

int User::getUserFd() const
{
	return this->_socketFd;
}

bool User::getAuth() const
{
	return this->_isAuth;
}
