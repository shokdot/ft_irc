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
	std::set<Channel *> _joinedChannels;
	bool _isAuth;
	bool _isQuitting;

public:
	Client(int fd);
	~Client();

	void setUsername(const String &username);
	void setNickname(const String &nickname);
	void setRealname(const String &realname);
	void setHostname(const String &hostname);
	void setAuth(bool auth);

	const String &getUsername() const;
	const String &getNickname() const;
	const String &getRealname() const;
	const String &getHostname() const;
	int getClientFd() const;
	bool getAuth() const;
	bool isRegistered() const;
	void joinChannel(Channel *channel);
	void removeChannel(Channel *channel);
	bool isJoinedChannel(Channel *channel);
	std::set<Channel *> &getJoinedChannels();
	void broadcastJoinedChannels(const String &msg);
	String getPrefix() const;
	bool isQuitting();
	void setIsQuitting(bool flag);
};

#endif
// CLIENT_HPP
