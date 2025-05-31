#include <MsgStrategy.hpp>
#include <IRCServer.hpp>

MsgStrategy::MsgStrategy() {}

MsgStrategy::~MsgStrategy() {}

void MsgStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	(void)pollManager;
	if (!readFromSock(fd, server))
		return;
	if (!checkBuffLength(fd, server))
		return;
	processMsg(fd, server);
}

void MsgStrategy::processMsg(int fd, IRCServer &server)
{
	String &data = sockBuffer[fd];
	size_t pos;
	while ((pos = data.find("\r\n")) != String::npos)
	{
		String line = Utils::trim(data.substr(0, pos));
		data.erase(0, pos + 2);
		if (line.length() > 510)
		{
			std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
			continue;
		}
		_dispatcher.dispatch(fd, line, server);
	}
}

bool MsgStrategy::readFromSock(int fd, IRCServer &server)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		disconnect(fd, bytes, server);
		return false;
	}
	sockBuffer[fd] += std::string(buffer, bytes);
	return true;
}

bool MsgStrategy::checkBuffLength(int fd, IRCServer &server)
{
	std::string &data = sockBuffer[fd];
	if (data.length() > 2048)
	{
		std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
		disconnect(fd, -1, server);
		return false;
	}
	return true;
}

void MsgStrategy::disconnect(int fd, int bytes, IRCServer &server)
{
	EventDispatcher &eventDispatcher = server.getEventDispatcher();

	if (bytes != 0)
		std::cerr << "[ERROR] Failed to recv client " << fd << ": " << strerror(errno) << std::endl;

	eventDispatcher.disconnectClient(fd, server);
}
