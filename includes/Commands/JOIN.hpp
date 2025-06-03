#ifndef JOIN_HPP
#define JOIN_HPP

#include <ACommand.hpp>

class JOIN : public ACommand
{
public:
	JOIN();
	~JOIN();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
