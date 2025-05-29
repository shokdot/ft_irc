#include <UserManager.hpp>

void UserManager::addUser(int fd, User *newUser)
{
	if (!newUser)
		return;
	String nickname = newUser->getNickname();
	_usersByFd[fd] = newUser;
	if (nickname != "*")
		_usersByNick[nickname] = newUser;
}

void UserManager::removeUser(int fd)
{
	(void)fd;
	// remove behavior
}

User *UserManager::createUser(int fd)
{
	return new User(fd);
}

User *UserManager::getUserByFd(int fd)
{
	std::map<int, User *>::iterator it = _usersByFd.find(fd);
	if (it != _usersByFd.end())
		return _usersByFd[fd];
	return NULL;
}

User *UserManager::getUserByNick(const String &nick)
{
	std::map<String, User *>::iterator it = _usersByNick.find(nick);
	if (it != _usersByNick.end())
		return _usersByNick[nick];
	return NULL;
}

bool UserManager::changeNick(const String &nickname, User *user)
{
	if (!user)
		return false;
	std::map<String, User *>::iterator it = _usersByNick.find(nickname);
	if (it != _usersByNick.end() && it->second != user)
		return false;
	const String &old_nick = user->getNickname();
	if (old_nick != "*")
		_usersByNick.erase(old_nick);
	user->setNickname(nickname);
	_usersByNick[nickname] = user;
	return true;
}
