#include <MsgStrategy.hpp>
#include <IRCServer.hpp>

MsgStrategy::MsgStrategy() {}

MsgStrategy::~MsgStrategy() {}

void MsgStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	UserManager &userManager = server.getUserManager();
	// ChannelManager &channelManager = server.getChannelManager();

	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		disconnect(fd, bytes, pollManager, userManager);
		return;
	}
	sockBuffer[fd] += std::string(buffer, bytes);
	std::string &data = sockBuffer[fd];
	if (data.length() > 2048)
	{
		std::cerr << "Client " << fd << " sent too much data without newline. Disconnecting" << std::endl; // change
		disconnect(fd, -1, pollManager, userManager);
		return;
	}
	size_t pos;
	while ((pos = data.find("\r\n")) != std::string::npos)
	{
		std::string line = data.substr(0, pos);
		data.erase(0, pos + 2); // remove the line and \r\n
		if (line.length() > 510)
		{
			std::cout << "Error too long" << std::endl; // change
			continue;
		}
		std::cout << "Message from " << fd << ": " << line << std::endl;
	}
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
