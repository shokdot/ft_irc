#include <EventDispatcher.hpp>

EventDispatcher::EventDispatcher() {}

void EventDispatcher::handleEvents()
{
	if (_pollManager.wait() < -1)
		throw IRCException::ServerError(std::strerror(errno));

	const std::vector<struct pollfd> &fds = _pollManager.getPollFds();
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].revents & POLL_IN)
		{
			// smth
		}
		else if (fds[i].revents & POLL_HUP)
		{
			// smth
		}
		else if (fds[i].revents & POLL_OUT)
		{
			// smth
		}
		else if (fds[i].revents & POLL_ERR)
		{
			// smth
		}
	}
}

// void EventHandler::handlePOLLIN(int index)
// {
// 	// if (_fds[index].fd == _serverFd)
// 	// 	handleNewConnection();
// 	// else
// 	// 	handleClientMessage(_fds[index].fd);
// }

// void EventHandler::handlePOLLERR(int index)
// {
// 	// std::cout << "Error or hangup on fd: " << _fds[index].fd << std::endl;
// 	// close(fds[index].fd);
// 	// fds.erase(fds.begin() + index);
// 	// --i; // Adjust index after erasing
// }

// void EventHandler::handleNewConnection()
// {
// 	struct sockaddr_in clientAddr;
// 	socklen_t len = sizeof(clientAddr);
// 	std::memset(&clientAddr, 0, len);
// 	int clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &len);
// 	if (clientFd < 0)
// 		throw IRCException::ServerError(strerror(errno));
// 	// addFd(clientFd, POLLIN);
// 	// _fds.push_back(createConnection(clientFd, POLLIN));
// 	// IdentService::identLookup(clientAddr, 6667);
// 	// std::cout << IdentService::reverseDNS(clientAddr) << std::endl;
// 	std::cout << "New client connected: " << clientFd << std::endl;
// }

// void EventHandler::handleClientMessage(int clientFd)
// {
// 	char buffer[1024];
// 	std::memset(buffer, 0, sizeof(buffer));
// 	int bytes = read(clientFd, buffer, sizeof(buffer) - 1);
// 	if (bytes <= 0)
// 	{
// 		std::cout << "Client disconnected: " << clientFd << std::endl;
// 		close(clientFd);
// 		// removeFd(clientFd)
// 		// for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
// 		// {
// 		// 	if (it->fd == clientFd)
// 		// 	{
// 		// 		_fds.erase(it);
// 		// 		break;
// 		// 	}
// 		// }
// 	}
// 	else
// 	{
// 		std::cout << "Message from " << clientFd << ": " << buffer;
// 		// TODO: Parse and respond to IRC commands
// 	}
// }

void EventDispatcher::init(int fd)
{
	this->_serverFd = fd;
	_pollManager.addFd(_serverFd, POLLIN);
}
