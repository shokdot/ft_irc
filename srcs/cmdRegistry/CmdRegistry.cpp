#include <CmdRegistry.hpp>

CmdRegistry::CmdRegistry()
{
	_commands["PING"] = new PING();
}

CmdRegistry::~CmdRegistry()
{
	std::unordered_map<String, ACommand *>::iterator it = _commands.begin();
	for (; it != _commands.end(); ++it)
		delete it->second;
	_commands.clear();
}

ACommand *CmdRegistry::getCmd(String cmdName)
{
	std::unordered_map<String, ACommand *>::iterator it = _commands.find(cmdName);
	return (it != _commands.end()) ? it->second : nullptr;
}
