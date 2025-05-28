#ifndef CMD_REGISTRY_HPP
#define CMD_REGISTRY_HPP

#include <IRC.hpp>
#include <PING.hpp>
#include <PASS.hpp>

class CmdRegistry
{
private:
	std::unordered_map<String, ACommand *> _commands;

public:
	CmdRegistry();
	~CmdRegistry();
	ACommand *getCmd(String cmdName);
};

#endif
