#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <IRC.hpp>

class Client
{
private:
	const int _socketFd;
	struct sockaddr_in _clientAddr;
	String _username;
	String _nickname;
	String _realname;
	String _hostname;
	std::set<Channel *> _joinedChannels;
	bool _isAuth;
	bool _isRegistered;

public:
	Client(int fd, struct sockaddr_in socketInfo);
	~Client();

	void setUsername(const String &username);
	void setNickname(const String &nickname);
	void setRealname(const String &realname);
	void setHostname();
	void setAuth(bool auth);
	void setIsRegistered(bool flag);

	int getClientFd() const;
	const String &getUsername() const;
	const String &getNickname() const;
	const String &getRealname() const;
	const String &getHostname() const;
	bool isJoinedChannel(Channel *channel) const;
	std::set<Channel *> &getJoinedChannels();
	bool getAuth() const;
	bool isRegistered() const;

	void joinChannel(Channel *channel);
	void removeFromChannel(Channel *channel);
	void broadcastJoinedChannels(const String &msg) const;
	void sendWelcome(const String &time) const;
	String getPrefix() const;

	bool hasNick() const;
	bool hasUser() const;

	bool isFirstLogin() const;
	void sendReply(const String &str) const;
};

#endif
// CLIENT_HPP
