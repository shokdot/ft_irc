#include <NICK.hpp>
#include <IRCServer.hpp>

NICK::NICK()
{
}

NICK::~NICK()
{
}

void NICK::execute(int fd, CmdStruct &cmd, IRCServer &server)
{
	UserManager &userManager = server.getUserManager();
	User *user = userManager.getUserByFd(fd);
	if (!user)
	{
		std::string reply = ":localhost 437 * :Nick is temporarily unavailable\r\n";
		Utils::sendWrapper(reply, fd);
		std::cout << "[ERROR] Client " << fd << " User not defined" << std::endl;
		return;
	}
	if (cmd.params.empty())
	{
		std::string reply = ":localhost 431 :No nickname given\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	String current_nick = user->getNickname();
	String nickname = Utils::strToLower(cmd.params[0]);
	if (nickname == current_nick)
		return;
	else if (!isValidNick(cmd.params[0]))
	{
		std::string reply = ":localhost 432 " + current_nick + " " + nickname + " :Erroneous nickname\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (!isNickAvalible(nickname, userManager))
	{
		std::string reply = ":localhost 433 " + nickname + " :Nickname is already in use\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else if (!userManager.changeNick(nickname, user))
		return;
	// broadcast messages
	std::cout << "nick changed" << std::endl;
}

bool NICK::isValidNick(const String &nickname)
{
	// trim
	if (nickname.empty() || nickname.size() > 9)
		return false;
	if (!(std::isalpha(static_cast<unsigned char>(nickname[0])) || isSpecial(static_cast<unsigned char>(nickname[0]))))
		return false;
	char c;
	for (size_t i = 0; i < nickname.size(); ++i)
	{
		c = static_cast<unsigned char>(nickname[i]);
		if (!(std::isalnum(c) || isSpecial(c) || c == '-'))
			return false;
	}
	return true;
}

bool NICK::isSpecial(char c)
{
	return (c >= '[' && c <= '`') || (c >= '{' && c <= '}');
}

bool NICK::isNickAvalible(const String &nickname, UserManager &userManager)
{
	if (userManager.getUserByNick(nickname))
		return false;
	return true;
}
