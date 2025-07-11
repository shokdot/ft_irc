#include <EventDispatcher.hpp>
#include <IRCServer.hpp>

EventDispatcher::EventDispatcher(IRCServer &server) : _server(server)
{
	_strategies[MESSAGE] = new MsgStrategy();
	_strategies[ERROR] = new ErrorStrategy();
	_strategies[NEWCONN] = new AcceptStrategy();
}

EventDispatcher::~EventDispatcher()
{
	for (std::map<int, IEventStrategy *>::iterator
			 it = _strategies.begin();
		 it != _strategies.end();
		 ++it)
	{
		delete it->second;
	}
	_strategies.clear();
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
		IEventStrategy *strategy = NULL;
		if ((fds[i].revents & POLLIN) && fds[i].fd == _serverFd)
			strategy = _strategies[NEWCONN];
		else if (fds[i].revents & POLLIN)
			strategy = _strategies[MESSAGE];
		else if (fds[i].revents & (POLLHUP | POLLERR))
			strategy = _strategies[ERROR];

		if (strategy)
			strategy->handleEvent(fds[i].fd, _pollManager, _server);
	}
}

void EventDispatcher::disconnectServer()
{
	if (_serverFd >= 0)
	{
		_pollManager.removeFd(_serverFd);
		if (close(_serverFd) < 0)
			throw IRCException::ServerError(std::strerror(errno));
	}
}

void EventDispatcher::disconnectClient(int fd)
{
	IEventStrategy *strategy = _strategies[ERROR];
	if (strategy)
		strategy->handleEvent(fd, _pollManager, _server);
}

void EventDispatcher::disconnectAllClients()
{
	std::vector<struct pollfd> clients = _pollManager.getPollFds();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		disconnectClient(clients[i].fd);
	}
}
