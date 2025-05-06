#include <EventHandler.hpp>

EventHandler::EventHandler() {}

struct pollfd EventHandler::createConnection(int fd, short events)
{
	struct pollfd pollStruct;
	pollStruct.fd = fd;
	pollStruct.events = events; // error and hangup events, also
	// POLLOUT for write check, for send response
	pollStruct.revents = 0; //  ?
	return pollStruct;
}

void EventHandler::handleEvents()
{
	fds.push_back(createConnection(serverFd, POLLIN));
	if (poll(&fds[0], fds.size(), -1) < 0)
		throw IRCException::ServerError(strerror(errno));

	for (size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i].revents & POLLIN)
			handlePOLLIN(i);
		else if (fds[i].revents & (POLLERR | POLL_HUP))
			handlePOLLERR(i);
	}
}

void EventHandler::handlePOLLIN(int index)
{
	if (fds[index].fd == serverFd)
		handleNewConnection();
	else
		handleClientMessage(fds[index].fd);
}

void EventHandler::handlePOLLERR(int index)
{
	std::cout << "Error or hangup on fd: " << fds[index].fd << std::endl;
	// close(fds[index].fd);
	// fds.erase(fds.begin() + index);
	// --i; // Adjust index after erasing
}

void EventHandler::handleNewConnection()
{
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	std::memset(&clientAddr, 0, len);
	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0)
		throw IRCException::ServerError(strerror(errno));
	fds.push_back(createConnection(clientFd, POLLIN));
	Ident::IdentLookup(clientAddr, 6667);
	std::cout << Ident::reverseDNS(clientAddr) << std::endl;
	std::cout << "New client connected: " << clientFd << std::endl;
}

void EventHandler::handleClientMessage(int clientFd)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = read(clientFd, buffer, sizeof(buffer) - 1);
	if (bytes <= 0)
	{
		std::cout << "Client disconnected: " << clientFd << std::endl;
		close(clientFd);

		for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
		{
			if (it->fd == clientFd)
			{
				fds.erase(it);
				break;
			}
		}
	}
	else
	{
		std::cout << "Message from " << clientFd << ": " << buffer;
		// TODO: Parse and respond to IRC commands
	}
}

void EventHandler::setServerFd(int fd)
{
	this->serverFd = fd;
}

int EventHandler::getServerFd()
{
	return this->serverFd;
}
