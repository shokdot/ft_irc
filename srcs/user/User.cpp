#include <User.hpp>

User::User(int socketFd) : socketFd(socketFd), isAuth(false)
{
}

User::~User()
{
}

void User::setUsername(const String &username)
{
	this->username = username;
}

void User::setNickname(const String &nickname)
{
	this->nickname = nickname;
}

void User::setRealname(const String &realname)
{
	this->realname = realname;
}

void User::setHostname(const String &hostname)
{
	this->hostname = hostname;
}

void User::setAuth(bool auth)
{
	this->isAuth = auth;
}

const String &User::getUsername() const
{
	return this->username;
}

const String &User::getNickname() const
{
	return this->nickname;
}

const String &User::getRealname() const
{
	return this->realname;
}

const String &User::getHostname() const
{
	return this->hostname;
}

int User::getUserFd() const
{
	return this->socketFd;
}

bool User::getAuth() const
{
	return this->isAuth;
}
