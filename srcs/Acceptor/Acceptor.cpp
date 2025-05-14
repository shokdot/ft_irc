#include <Acceptor.hpp>

#include <IRC.hpp>

int Acceptor::acceptConnection(int serverFd)
{
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	std::memset(&clientAddr, 0, len);
	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0)
		throw IRCException::ServerError(strerror(errno));
	std::cout << "[INFO] Client connected; fd -> " << clientFd << std::endl;
	return clientFd;
	// _fds.push_back(createConnection(clientFd, POLLIN));
	// IdentService::identLookup(clientAddr, 6667);
	// std::cout << IdentService::reverseDNS(clientAddr) << std::endl;
}
