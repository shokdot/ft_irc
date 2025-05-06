#include <IRCServer.hpp>

IRCServer::IRCServer(std::pair<int, String> pair) : port(pair.first), password(pair.second)
{
	this->setup();
	this->run();
}

IRCServer::~IRCServer()
{
	if (close(server_fd) < 0)
		throw IRCException::ServerError(std::strerror(errno));
}

void IRCServer::setup()
{
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
		throw IRCException::ServerError(std::strerror(errno));
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	if (listen(server_fd, MAX_CONN) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	eventhandler.setServerFd(this->server_fd);
}

void IRCServer::run()
{
	while (true)
	{
		eventhandler.handleEvents();
	}
}
