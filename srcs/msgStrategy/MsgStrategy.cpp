#include <MsgStrategy.hpp>
#include <IRCServer.hpp>

MsgStrategy::MsgStrategy() {}

MsgStrategy::~MsgStrategy() {}

void MsgStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	(void)pollManager;
	EventDispatcher &eventDispatcher = server.getEventDispatcher();

	if (!readFromSock(fd, eventDispatcher))
		return;
	if (!checkBuffLength(fd, eventDispatcher))
		return;
	processMsg(fd, server);
}

void MsgStrategy::processMsg(int fd, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	Client *client = clientManager.getClientByFd(fd);
	String &data = _sockBuffer[fd];
	size_t pos;
	while ((pos = data.find("\r\n")) != String::npos)
	{
		if (!clientManager.getClientByFd(fd))
		{
			_sockBuffer.erase(fd);
			return;
		}
		String line = Utils::trim(data.substr(0, pos));
		std::cout << line << std::endl;
		data.erase(0, pos + 2);
		if (line.length() > 510)
		{
			std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
			continue;
		}
		_dispatcher.dispatch(client, line, server);
	}
}

bool MsgStrategy::readFromSock(int fd, EventDispatcher &eventDispatcher)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		disconnect(fd, bytes, eventDispatcher);
		return false;
	}
	_sockBuffer[fd] += String(buffer, bytes);
	return true;
}

bool MsgStrategy::checkBuffLength(int fd, EventDispatcher &eventDispatcher)
{
	std::string &data = _sockBuffer[fd];
	if (data.length() > 2048)
	{
		std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
		disconnect(fd, -1, eventDispatcher);
		return false;
	}
	return true;
}

void MsgStrategy::disconnect(int fd, int bytes, EventDispatcher &eventDispatcher)
{
	if (bytes != 0)
		std::cerr << "[ERROR] Failed to recv client " << fd << ": " << strerror(errno) << std::endl;
	_sockBuffer.erase(fd);
	eventDispatcher.disconnectClient(fd);
}
