#include <PING.hpp>
#include <IRCServer.hpp>

PING::PING()
{
}

PING::~PING()
{
}

void PING::execute(int fd, FullCmd &cmd, IRCServer &server)
{
	(void)server;
	(void)cmd;
	Utils::sendWrapper("PONG\n", fd);
}
