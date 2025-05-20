#include <AcceptStrategy.hpp>

AcceptStrategy::AcceptStrategy()
{
}

AcceptStrategy::~AcceptStrategy()
{
}

void AcceptStrategy::handleEvent(int fd __unused, PollManager &pollManager __unused, IRCServer &server __unused)
{
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	std::memset(&clientAddr, 0, len);
	int clientFd = accept(fd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0 || fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
		throw IRCException::ServerError(strerror(errno));
	pollManager.addFd(clientFd, POLL_IN);
	std::cout << "[INFO] Client " << clientFd << " connected" << std::endl;
}
