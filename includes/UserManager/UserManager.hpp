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
	void print()
	{
		std::map<int, User *>::iterator it = _usersByFd.begin();
		for (; it != _usersByFd.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second << std::endl;
		}
		std::map<String, User *>::iterator it2 = _usersByNick.begin();
		for (; it2 != _usersByNick.end(); ++it2)
		{
			std::cout << it2->first << "\t" << it2->second << std::endl;
		}
	}
};

#endif
// USER_MANAGER_HPP
