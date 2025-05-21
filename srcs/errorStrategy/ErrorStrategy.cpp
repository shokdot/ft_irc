#include <ErrorStrategy.hpp>
#include <IRCServer.hpp>

ErrorStrategy::ErrorStrategy() {}

ErrorStrategy::~ErrorStrategy() {}

void ErrorStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	UserManager &userManager = server.getUserManager();
	if (close(fd) < 0)
		std::cerr << "[ERROR] Failed to close client " << fd << " : " << strerror(errno) << std::endl;
	pollManager.removeFd(fd);
	userManager.removeUser(fd);
	std::cout << "[INFO] Client " << fd << " disconnected" << std::endl;
}
