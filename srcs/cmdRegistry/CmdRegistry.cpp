#include <CmdRegistry.hpp>

CmdRegistry::CmdRegistry()
{
	_commands["PASS"] = new PASS();
	_commands["NICK"] = new NICK();
	_commands["USER"] = new USER();
	_commands["JOIN"] = new JOIN();
	_commands["PART"] = new PART();
	// _commands["KICK"] = new KICK();
	_commands["INVITE"] = new INVITE();
	_commands["TOPIC"] = new TOPIC();
	// _commands["MODE"] = new MODE();
	// _commands["PRIVMSG"] = new PRIVMSG();
	_commands["QUIT"] = new QUIT();
	// _commands["CAP"] = new CAP();
}

CmdRegistry::~CmdRegistry()
{
	std::map<String, ACommand *>::iterator it = _commands.begin();
	for (; it != _commands.end(); ++it)
		delete it->second;
	_commands.clear();
}

ACommand *CmdRegistry::getCmd(String cmdName)
{
	std::map<String, ACommand *>::iterator it = _commands.find(cmdName);
	return (it != _commands.end()) ? it->second : NULL;
}
