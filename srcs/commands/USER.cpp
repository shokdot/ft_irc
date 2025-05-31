#include <USER.hpp>

USER::USER()
{
}

USER::~USER()
{
}

void USER::execute(int fd, CmdStruct &cmd, IRCServer &server)
{
	(void)server;
	(void)cmd;
	(void)fd;
	std::cout << "lol" << std::endl;
}
