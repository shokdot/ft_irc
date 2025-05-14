#include <EventDispatcher.hpp>

EventDispatcher::EventDispatcher(IRCServer *server) : _server(server)
{
	_cmdHandler.setServer(_server);
}

void EventDispatcher::init(int fd)
{
	this->_serverFd = fd;
	_pollManager.addFd(_serverFd, POLLIN);
}

void EventDispatcher::handleEvents()
{
	if (_pollManager.wait() < 0)
		throw IRCException::ServerError(std::strerror(errno));

	const std::vector<struct pollfd> &fds = _pollManager.getPollFds();
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].revents & POLL_IN)
			handlePollIn(fds[i].fd);
		else if (fds[i].revents & (POLL_HUP | POLL_ERR))
			handlePollErr(fds[i].fd);
		else if (fds[i].revents & POLL_OUT)
			handlePollOut(fds[i].fd);
	}
}

void EventDispatcher::handlePollErr(int fd)
{
	std::cout << "Error or Disconected fd -> " << fd << std::endl;
	_pollManager.removeFd(fd);
}

void EventDispatcher::handlePollIn(int fd)
{
	if (fd == _serverFd)
	{
		_pollManager.addFd(_acceptor.acceptConnection(fd), POLL_IN);
	}
	else
	{
	}
}

void EventDispatcher::handlePollOut(int fd)
{
	std::cout << "do smth. " << fd << std::endl;
}

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
