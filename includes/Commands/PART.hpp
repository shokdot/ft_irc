#ifndef PART_HPP
#define PART_HPP

#include <ACommand.hpp>

class PART : public ACommand
{
public:
	PART();
	~PART();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
