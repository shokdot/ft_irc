#include <IRCServer.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

IRCServer::IRCServer(int port, String password) : _port(port), _password(password), _serverFd(-1), _running(false), _eventDispatcher(*this)
{
	std::time_t now = std::time(NULL);
	_creationTime = std::ctime(&now);
}

IRCServer::~IRCServer() throw()
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

	struct sockaddr_in sockStruct = Utils::createSockStruct(AF_INET, _port, INADDR_ANY);
	if (bind(_serverFd, (struct sockaddr *)&sockStruct, sizeof(sockStruct)) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	if (listen(_serverFd, MAX_CONN) < 0)
		throw IRCException::ServerError(std::strerror(errno));
	_eventDispatcher.init(this->_serverFd);
	std::cout << "[INFO] Server running on port " << _port << std::endl;
}

void IRCServer::run()
{
	_running = true;
	while (_running)
	{
		_eventDispatcher.handleEvents();
	}
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

ClientManager &IRCServer::getClientManager() { return _clientManager; }

ChannelManager &IRCServer::getChannelManager() { return _channelManager; }

EventDispatcher &IRCServer::getEventDispatcher() { return _eventDispatcher; }

const String &IRCServer::getPass() { return _password; }

const String &IRCServer::getCreationTime() { return _creationTime; }
