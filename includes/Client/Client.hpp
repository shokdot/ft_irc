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
	bool _isRegistered;

public:
	Client(int fd);
	~Client();

	void setUsername(const String &username);
	void setNickname(const String &nickname);
	void setRealname(const String &realname);
	void setHostname(const String &hostname);
	void setAuth(bool auth);
	void setIsQuitting(bool flag);
	void setIsRegistered(bool flag);

	int getClientFd() const;
	const String &getUsername() const;
	const String &getNickname() const;
	const String &getRealname() const;
	const String &getHostname() const;
	bool isJoinedChannel(Channel *channel);
	std::set<Channel *> &getJoinedChannels();
	bool getAuth() const;
	bool isRegistered() const;
	bool isQuitting();

	void joinChannel(Channel *channel);
	void removeChannel(Channel *channel);
	void broadcastJoinedChannels(const String &msg);
	void sendWelcome(const String &time);
	String getPrefix() const;

	bool hasNick();
	bool hasUser();

	bool isFirstLogin();
	void sendReply(const String &str);
};

#endif
// CLIENT_HPP
