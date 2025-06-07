#include <Replies.hpp>

namespace Reply
{
	const String servername = "irc.42.chat";

	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName)
	{
		return ":" + servername + " 461 " + nickname + " " + cmdName + " :Not enough parameters";
	}

	String ERR_ALREADYREGISTRED(const String &nickname)
	{
		return ":" + servername + " 462 " + nickname + " :You may not reregister";
	}

	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason)
	{
		return ":" + servername + " 464 " + nickname + " :Password " + reason;
	}

	String ERR_NONICKNAMEGIVEN(const String &nickname)
	{
		return ":" + servername + " 431 " + nickname + " :No nickname given";
	}

	String ERR_ERRONEUSNICKNAME(const String &oldNick, const String &newNick)
	{
		return ":" + servername + " 432 " + oldNick + " " + newNick + " :Erroneous nickname";
	}

	String ERR_NICKNAMEINUSE(const String &oldNick, const String &newNick)
	{
		return ":" + servername + " 433 " + oldNick + " " + newNick + " :Nickname is already in use";
	}

	String RPL_SUCCNICK(const String &prefix, const String &newNick)
	{
		return ":" + prefix + " NICK :" + newNick;
	}

	String RPL_QUIT(const String &prefix, const String &msg)
	{
		return ":" + prefix + " QUIT :" + msg;
	}

	String ERR_UNKNOWNCOMMAND(const String &nickname, const String &cmd)
	{
		return ":" + servername + " 421 " + nickname + " " + cmd + " :Unknown command";
	}

}
