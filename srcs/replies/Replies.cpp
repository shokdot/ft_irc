#include <Replies.hpp>

namespace Reply
{
	const String servername = "irc.42.chat";

	String RPL_WELCOME(const String &nickname, const String &prefix)
	{
		return ":" + servername + " 001 " + nickname + " :Welcome to the Internet Relay Network " + prefix;
	}

	String RPL_YOURHOST(const String &nickname)
	{
		return ":" + servername + " 002 " + nickname + " :Your host is " + servername + ", running version ft_irc-0.1";
	}

	String RPL_CREATED(const String &nickname, const String &time)
	{
		return ":" + servername + " 003 " + nickname + " :Your host is " + servername + ", created on " + time;
	}

	String RPL_MYINFO(const String &nickname)
	{
		return ":" + servername + " 004 " + nickname + " " + servername + " ft_irc-0.1 o itklo";
	}

	String ERR_NOSUCHCHANNEL(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 403 " + nickname + " " + channelName + " :No such channel";
	}

	String ERR_UNKNOWNCOMMAND(const String &nickname, const String &cmd)
	{
		return ":" + servername + " 421 " + nickname + " " + cmd + " :Unknown command";
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

	String ERR_NOTREGISTERED(const String &nickname)
	{
		return ":" + servername + " 451 " + nickname + " :You have not registered";
	}

	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName)
	{
		return ":" + servername + " 461 " + nickname + " " + cmdName + " :Not enough parameters";
	}

	String ERR_ALREADYREGISTRED(const String &nickname)
	{
		return ":" + servername + " 462 " + nickname + " :Unauthorized command (already registered)";
	}

	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason)
	{
		return ":" + servername + " 464 " + nickname + " :Password " + reason;
	}

	String ERR_INVALIDUSERNAME(const String &username)
	{
		return ":" + servername + " 468 " + username + " :Invalid username";
	}

	String RPL_SUCCNICK(const String &prefix, const String &newNick)
	{
		return ":" + prefix + " NICK :" + newNick;
	}

	String RPL_QUIT(const String &prefix, const String &msg)
	{
		return ":" + prefix + " QUIT :" + msg;
	}
}
