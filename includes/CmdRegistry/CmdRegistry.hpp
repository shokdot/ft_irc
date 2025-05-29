#ifndef CMD_REGISTRY_HPP
#define CMD_REGISTRY_HPP

#include <IRC.hpp>
#include <PING.hpp>
#include <PASS.hpp>
#include <NICK.hpp>

class CmdRegistry
{
private:
	std::map<String, ACommand *> _commands;

public:
	CmdRegistry();
	~CmdRegistry();
	ACommand *getCmd(String cmdName);
};

#endif
