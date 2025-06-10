#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <ACommand.hpp>

class TOPIC : public ACommand
{
public:
	TOPIC();
	~TOPIC();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};
#endif
