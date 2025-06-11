#ifndef CMD_REGISTRY_HPP
#define CMD_REGISTRY_HPP

#include <IRC.hpp>
#include <PASS.hpp>
#include <NICK.hpp>
#include <USER.hpp>
#include <QUIT.hpp>
#include <JOIN.hpp>
#include <PART.hpp>
#include <INVITE.hpp>
#include <TOPIC.hpp>
#include <KICK.hpp>

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
