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
	std::map<int, Client *>::iterator it = _clientsByFd.find(fd);
	if (it == _clientsByFd.end())
		return;

	Client *client = it->second;
	String nickname = client->getNickname();
	if (nickname != "*")
		_clientsByNick.erase(nickname);
	delete client;
	_clientsByFd.erase(it);
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
	const String &oldNick = client->getNickname();
	if (oldNick != "*")
		_clientsByNick.erase(oldNick);
	client->setNickname(nickname);
	_clientsByNick[nickname] = client;
	return true;
}
