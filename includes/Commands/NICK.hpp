#ifndef NICK_HPP
#define NICK_HPP

#include <ACommand.hpp>

class NICK : public ACommand
{
public:
	NICK();
	~NICK();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);

private:
	bool isValidNick(const String &nickname);
	bool isNickAvalible(const String &nickname, ClientManager &clientManager);
	bool isSpecial(char c);
};

#endif
