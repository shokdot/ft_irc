#include <CmdHandler.hpp>

void CmdHandler::execute()
{
	std::cout << "smth" << std::endl;
}

void CmdHandler::setServer(IRCServer *server)
{
	this->_server = server;
}
