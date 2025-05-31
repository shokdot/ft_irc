#include <ClientManager.hpp>

void ClientManager::addClient(int fd, Client *newClient)
{
	if (!newClient)
		return;
	String nickname = newClient->getNickname();
	_clientsByFd[fd] = newClient;
	if (nickname != "*")
		_clientsByNick[nickname] = newClient;
}

void ClientManager::removeClient(int fd)
{
	(void)fd;
	// remove behavior
}

Client *ClientManager::createClient(int fd)
{
	return new Client(fd);
}

Client *ClientManager::getClientByFd(int fd)
{
	std::map<int, Client *>::iterator it = _clientsByFd.find(fd);
	if (it != _clientsByFd.end())
		return _clientsByFd[fd];
	return NULL;
}

Client *ClientManager::getClientByNick(const String &nick)
{
	std::map<String, Client *>::iterator it = _clientsByNick.find(nick);
	if (it != _clientsByNick.end())
		return _clientsByNick[nick];
	return NULL;
}

bool ClientManager::changeNick(const String &nickname, Client *client)
{
	if (!client)
		return false;
	std::map<String, Client *>::iterator it = _clientsByNick.find(nickname);
	if (it != _clientsByNick.end() && it->second != client)
		return false;
	const String &old_nick = client->getNickname();
	if (old_nick != "*")
		_clientsByNick.erase(old_nick);
	client->setNickname(nickname);
	_clientsByNick[nickname] = client;
	return true;
}
