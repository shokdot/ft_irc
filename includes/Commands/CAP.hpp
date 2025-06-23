#ifndef CAP_HPP
#define CAP_HPP

#include <ACommand.hpp>

class CAP : public ACommand
{
public:
	CAP();
	~CAP();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);
};

#endif
