#ifndef USER_HPP
#define USER_HPP

#include <ACommand.hpp>

class USER : public ACommand
{
private:
	bool isValidUsername(const String &username);

public:
	USER(/* args */);
	~USER();
	void execute(int fd, CmdStruct &cmd, IRCServer &server);
};

#endif
