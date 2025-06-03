#ifndef QUIT_HPP
#define QUIT_HPP

#include <ACommand.hpp>

class QUIT : public ACommand
{
public:
	QUIT();
	~QUIT();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
