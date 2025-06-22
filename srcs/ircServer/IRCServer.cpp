#include <IRCServer.hpp>

IRCServer::IRCServer(int port, String password) : _port(port), _password(password), _serverFd(-1), _eventDispatcher(*this)
{
	std::time_t now = std::time(NULL);
	_creationTime = std::ctime(&now);
	_creationTime.pop_back();
}

IRCServer::~IRCServer() throw()
{
	try
	{
		stop();
	}
	catch (...)
	{
		std::cerr << "[ERROR] Something went wrong" << '\n';
	}
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
	while (g_running)
	{
		_eventDispatcher.handleEvents();
	}
}

void IRCServer::stop()
{
	std::cout << std::endl;
	_eventDispatcher.disconnectServer();
	_eventDispatcher.disconnectAllClients();
}

ClientManager &IRCServer::getClientManager() { return _clientManager; }

ChannelManager &IRCServer::getChannelManager() { return _channelManager; }

EventDispatcher &IRCServer::getEventDispatcher() { return _eventDispatcher; }

const String &IRCServer::getPass() { return _password; }

const String &IRCServer::getCreationTime() { return _creationTime; }
