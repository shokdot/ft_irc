#include <CmdDispatcher.hpp>

void CmdDispatcher::dispatch(int fd, String line, IRCServer &server)
{
	CmdStruct command;
	bool isValidCmd = CmdParser::parseCmd(line, command);
	if (!isValidCmd)
	{
		std::cout << "[WARNING] Client " << fd << " send invalid command" << std::endl;
		return;
	}
	ACommand *handler = _registry.getCmd(command.cmdName);
	if (handler)
		handler->execute(fd, command, server);
	else
	{
		std::string reply = ":localhost 421 " + command.cmdName + " :Unknown command\r\n";
		Utils::sendWrapper(reply, fd);
		std::cout << "[WARNING] Client " << fd << " send unrecognized command" << std::endl;
		return;
	}
}
