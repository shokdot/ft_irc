#include <UserManager.hpp>

void UserManager::addUser(int fd, User *newUser)
{
	_usersByFd[fd] = newUser;
}

void UserManager::removeUser(int fd)
{
	// remove behavior
}
