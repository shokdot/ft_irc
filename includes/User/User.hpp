#ifndef USER_HPP
#define USER_HPP

#include <IRC.hpp>

class User
{
private:
	String username;
	String nickname;
	String realname;
	String hostname;
	int socketFd;
	bool isAuth;

public:
	User(int);
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
