#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <IRC.hpp>

class Client
{
private:
	const int _socketFd;
	String _username;
	String _nickname;
	String _realname;
	String _hostname;
	bool _isAuth;

public:
	Client(int fd);
	~Client();

	void setUsername(const String &);
	void setNickname(const String &);
	void setRealname(const String &);
	void setHostname(const String &);
	void setAuth(bool);

	const String &getUsername() const;
	const String &getNickname() const;
	const String &getRealname() const;
	const String &getHostname() const;
	int getClientFd() const;
	bool getAuth() const;
};

#endif
// CLIENT_HPP
