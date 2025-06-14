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
	void print()
	{
		std::map<int, Client *>::iterator it = _clientsByFd.begin();
		for (; it != _clientsByFd.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second << std::endl;
		}
		std::map<String, Client *>::iterator it2 = _clientsByNick.begin();
		for (; it2 != _clientsByNick.end(); ++it2)
		{
			std::cout << it2->first << "\t" << it2->second << std::endl;
		}
	}
};

#endif
// CLIENT_MANAGER_HPP
