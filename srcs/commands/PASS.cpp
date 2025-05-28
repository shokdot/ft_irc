#include <PASS.hpp>
#include <IRCServer.hpp>

PASS::PASS()
{
}

PASS::~PASS()
{
}

void PASS::execute(int fd, CmdStruct &cmd, IRCServer &server)
{
	UserManager &userManager = server.getUserManager();
	User *user = userManager.getUserByFd(fd);
	if (cmd.params.size() < 1)
	{
		std::string reply = ":localhost 461 pass :Not enough parameters\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	if (cmd.params[0] != server.getPass())
	{
		std::string reply = ":localhost 464 :Password incorrect\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	if (!user)
	{
		std::string reply = "Something went wrong\r\n";
		Utils::sendWrapper(reply, fd);
		return;
	}
	else
	{
		if (user->getAuth())
		{
			std::string reply = ":localhost 462 " + user->getNickname() + " :You may not reregister\r\n";
			Utils::sendWrapper(reply, fd);
			return;
		}
		user->setAuth(true);
		std::cout << "[INFO] Client " << fd << " registered" << std::endl;
	}
}
