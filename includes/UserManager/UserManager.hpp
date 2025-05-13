#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <IRC.hpp>
#include <User.hpp>

class UserManager
{
private:
	std::map<int, User *> _usersByFd;
	std::map<String, User *> _usersByNick;

public:
	void addUser(int fd, User *);
	void removeUser(int fd);
};

#endif
