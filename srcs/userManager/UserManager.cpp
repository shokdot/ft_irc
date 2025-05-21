#include <UserManager.hpp>

void UserManager::addUser(int fd, User *newUser)
{
	if (!newUser)
		return;
	String nickname = newUser->getNickname();
	_usersByFd[fd] = newUser;
	if (nickname != "unknown")
		_usersByNick[nickname] = newUser;
}

void UserManager::removeUser(int fd)
{
	// remove behavior
}

User *UserManager::createUser(int fd)
{
	return new User(fd);
}
