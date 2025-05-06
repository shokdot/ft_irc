#include <IRCServer.hpp>

IRCServer::IRCServer(std::pair<int, String> pair) : _port(pair.first), _password(pair.second)
{
	this->setup();
	this->run();
}

IRCServer::~IRCServer()
{
	if (close(_serverFd) < 0)
		throw IRCException::ServerError(std::strerror(errno));
}

void IRCServer::setup()
{
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
		throw IRCException::ServerError(std::strerror(errno));
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;
	if (bind(_serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	if (listen(_serverFd, MAX_CONN) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	_eventHandler.setServerFd(this->_serverFd);
}

void IRCServer::run()
{
	while (true)
	{
		_eventHandler.handleEvents();
	}
}
