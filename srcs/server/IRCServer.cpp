#include <IRCServer.hpp>

IRCServer::IRCServer(int port, String password) : _port(port), _password(password), _serverFd(-1), _running(false), _eventDispatcher(this) {}

IRCServer::~IRCServer()
{
	stop();
}

void IRCServer::start()
{
	this->setup();
	this->run();
}

void IRCServer::setup()
{
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
		throw IRCException::ServerError(std::strerror(errno));

	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw IRCException::ServerError(std::strerror(errno));
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0)
		throw IRCException::ServerError(std::strerror(errno));

	struct sockaddr_in sockStruct = createSockStruct(AF_INET, _port, INADDR_ANY);
	if (bind(_serverFd, (struct sockaddr *)&sockStruct, sizeof(sockStruct)) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	if (listen(_serverFd, MAX_CONN) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	_eventDispatcher.init(this->_serverFd);
}

void IRCServer::run()
{
	_running = true;
	while (_running)
	{
		_eventDispatcher.handleEvents();
	}
}

struct sockaddr_in IRCServer::createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr)
{
	struct sockaddr_in sockStruct;
	sockStruct.sin_family = family;
	sockStruct.sin_port = htons(port);
	sockStruct.sin_addr.s_addr = addr;
	return sockStruct;
}

void IRCServer::stop()
{
	_running = false;
	if (_serverFd >= 0)
	{
		if (close(_serverFd) < 0)
			throw IRCException::ServerError(std::strerror(errno));
		_serverFd = -1;
	}
}
