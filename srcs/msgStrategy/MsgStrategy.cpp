#include <MsgStrategy.hpp>
#include <IRCServer.hpp>

MsgStrategy::MsgStrategy() {}

MsgStrategy::~MsgStrategy() {}

void MsgStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	UserManager &userManager = server.getUserManager();
	// ChannelManager &channelManager = server.getChannelManager();

	if (!readFromSock(fd, pollManager, userManager))
		return;
	if (!checkBuffLength(fd, pollManager, userManager))
		return;
	processMsg(fd, server);
}

void MsgStrategy::processMsg(int fd, IRCServer &server)
{
	String &data = sockBuffer[fd];
	size_t pos;
	while ((pos = data.find("\r\n")) != String::npos)
	{
		String line = data.substr(0, pos);
		data.erase(0, pos + 2);
		if (line.length() > 510)
		{
			std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
			continue;
		}
		std::cout << "Message from " << fd << ": " << line << std::endl;
		_dispatcher.dispatch(fd, line, server);
	}
}

bool MsgStrategy::readFromSock(int fd, PollManager &pollManager, UserManager &userManager)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		disconnect(fd, bytes, pollManager, userManager);
		return false;
	}
	sockBuffer[fd] += std::string(buffer, bytes);
	return true;
}

bool MsgStrategy::checkBuffLength(int fd, PollManager &pollManager, UserManager &userManager)
{
	std::string &data = sockBuffer[fd];
	if (data.length() > 2048)
	{
		std::cout << "[WARNING] Client " << fd << " sent too much data" << std::endl;
		disconnect(fd, -1, pollManager, userManager);
		return false;
	}
	return true;
}

void MsgStrategy::disconnect(int fd, int bytes, PollManager &pollManager, UserManager &userManager)
{
	if (close(fd) < 0)
		std::cerr << "[ERROR] Failed to close client " << fd << ": " << strerror(errno) << std::endl;

	if (bytes == 0)
		std::cout << "[INFO] client " << fd << " disconnected" << std::endl;
	else
		std::cerr << "[ERROR] Failed to recv client " << fd << ": " << strerror(errno) << std::endl;

	pollManager.removeFd(fd);
	userManager.removeUser(fd);
}
