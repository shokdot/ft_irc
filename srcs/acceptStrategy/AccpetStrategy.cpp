#include <AcceptStrategy.hpp>
#include <IRCServer.hpp>

AcceptStrategy::AcceptStrategy() {}

AcceptStrategy::~AcceptStrategy() {}

void AcceptStrategy::handleEvent(int fd, PollManager &pollManager, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();

	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	std::memset(&clientAddr, 0, len);

	int clientFd = accept(fd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0)
		throw IRCException::ServerError(strerror(errno));

	pollManager.addFd(clientFd, POLLIN);
	clientManager.addClient(clientFd, clientManager.createClient(clientFd, clientAddr));
	std::cout << "[INFO] Client " << clientFd << " connected" << std::endl;
}
