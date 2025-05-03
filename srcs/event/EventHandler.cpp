#include <EventHandler.hpp>

EventHandler::EventHandler(int serverFd) : _serverFd(serverFd)
{
	struct pollfd serverPollFd;
	serverPollFd.fd = _serverFd;
	serverPollFd.events = POLLIN;
	serverPollFd.revents = 0;
	_fds.push_back(serverPollFd);
}

void EventHandler::handleEvents()
{
	if (poll(&_fds[0], _fds.size(), -1) < 0)
	{
		perror("poll");
		return;
	}

	for (size_t i = 0; i < _fds.size(); ++i)
	{
		if (_fds[i].revents & POLLIN)
		{
			if (_fds[i].fd == _serverFd)
			{
				handleNewConnection();
			}
			else
			{
				handleClientMessage(_fds[i].fd);
			}
		}
	}
}

void EventHandler::handleNewConnection()
{
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	socklen_t len = sizeof(clientAddr);
	int clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0)
	{
		perror("accept");
		return;
	}

	struct pollfd clientPollFd;
	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	_fds.push_back(clientPollFd);

	std::cout << "New client connected: " << clientFd << std::endl;
}

void EventHandler::handleClientMessage(int clientFd)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	int bytes = read(clientFd, buffer, sizeof(buffer) - 1);
	if (bytes <= 0)
	{
		std::cout << "Client disconnected: " << clientFd << std::endl;
		close(clientFd);

		for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
		{
			if (it->fd == clientFd)
			{
				_fds.erase(it);
				break;
			}
		}
	}
	else
	{
		std::cout << "Message from " << clientFd << ": " << buffer;
		// TODO: Parse and respond to IRC commands
	}
}
