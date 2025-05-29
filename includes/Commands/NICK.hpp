#ifndef NICK_HPP
#define NICK_HPP

#include <ACommand.hpp>

class NICK : public ACommand
{
public:
	NICK(/* args */);
	~NICK();
	void execute(int fd, CmdStruct &cmd, IRCServer &server);

private:
	bool isValidNick(const String &nickname);
	bool isNickAvalible(const String &nickname, UserManager &userManager);
	bool isSpecial(char c);
};

#endif
