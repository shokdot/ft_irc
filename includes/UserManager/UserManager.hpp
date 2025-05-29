#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

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
	User *createUser(int fd);
	User *getUserByFd(int fd);
	User *getUserByNick(const String &nick);
	bool changeNick(const String &nickname, User *user);
};

#endif
// USER_MANAGER_HPP
