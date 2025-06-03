#ifndef PASS_HPP
#define PASS_HPP

#include <ACommand.hpp>

class PASS : public ACommand
{
public:
	PASS();
	~PASS();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
