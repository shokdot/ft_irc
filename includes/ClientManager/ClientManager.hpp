#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <IRC.hpp>
#include <Client.hpp>

class ClientManager
{
private:
	std::map<int, Client *> _clientsByFd;
	std::map<String, Client *> _clientsByNick;

public:
	void addClient(int fd, Client *);
	void removeClient(int fd);
	Client *createClient(int fd, struct sockaddr_in clientAddr);
	Client *getClientByFd(int fd);
	Client *getClientByNick(const String &nick);
	bool changeNick(const String &nickname, Client *client);
};

#endif
// CLIENT_MANAGER_HPP
