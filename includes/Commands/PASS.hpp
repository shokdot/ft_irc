#ifndef PASS_HPP
#define PASS_HPP

#include <ACommand.hpp>

class PASS : public ACommand
{
public:
	PASS(/* args */);
	~PASS();
	void execute(int fd, CmdStruct &cmd, IRCServer &server);
};

#endif
