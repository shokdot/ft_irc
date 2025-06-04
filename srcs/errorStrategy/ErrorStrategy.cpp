#include <ErrorStrategy.hpp>
#include <IRCServer.hpp>

ErrorStrategy::ErrorStrategy() {}

ErrorStrategy::~ErrorStrategy() {}

void ErrorStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	Client *client = clientManager.getClientByFd(fd);

	server.getChannelManager().partAll(client);
	pollManager.removeFd(fd);
	clientManager.removeClient(fd);

	if (close(fd) < 0)
		std::cerr << "[ERROR] Failed to close client " << fd << ": " << strerror(errno) << std::endl;

	std::cout << "[INFO] Client " << fd << " disconnected" << std::endl;
}
