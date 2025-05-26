#include <CmdDispatcher.hpp>

void CmdDispatcher::dispatch(int fd, String line, IRCServer &server)
{
	FullCmd command;
	bool isValidCmd = CmdParser::parseCmd(line, command);
	if (!isValidCmd)
		return;
	ACommand *handler = _registry.getCmd(command.cmdName);
	if (handler)
		handler->execute(fd, command, server);
	else
		std::cout << "Command not found!" << std::endl;
}
