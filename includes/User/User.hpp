#ifndef USER_HPP
#define USER_HPP

#include <IRC.hpp>

class User
{
private:
	const int _socketFd;
	String _username;
	String _nickname;
	String _realname;
	String _hostname;
	bool _isAuth;

public:
	User(int fd);
	~User();

	void setUsername(const String &);
	void setNickname(const String &);
	void setRealname(const String &);
	void setHostname(const String &);
	void setAuth(bool);

	const String &getUsername() const;
	const String &getNickname() const;
	const String &getRealname() const;
	const String &getHostname() const;
	int getUserFd() const;
	bool getAuth() const;
};

#endif
// USER_HPP
