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

	String RPL_NOTOPIC(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 331 " + nickname + " " + channelName + " :No topic is set";
	}

	String RPL_TOPIC(const String &nickname, const String &channelName, const String &topic)
	{
		return ":" + servername + " 332 " + nickname + " " + channelName + " :" + topic;
	}

	String RPL_INVITING(const String &nickname, const String &targetNick, const String &channelName)
	{
		return ":" + servername + " 341 " + nickname + " " + targetNick + " " + channelName;
	}

	String RPL_NAMREPLY(const String &nickname, const String &channelName, const String &userList)
	{
		return ":" + servername + " 353 " + nickname + " = " + channelName + " :" + userList;
	}

	String RPL_ENDOFNAMES(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 366 " + nickname + " " + channelName + " :End of /NAMES list";
	}

	String ERR_NOSUCHNICK(const String &nickname, const String &targetNick)
	{
		return ":" + servername + " 401 " + nickname + " " + targetNick + " :No such nick/channel";
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

	String ERR_USERNOTINCHANNEL(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 441 " + nickname + " " + channelName + " :They aren't on that channel";
	}

	String ERR_NOTONCHANNEL(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 442 " + nickname + " " + channelName + " :You're not on that channel";
	}

	String ERR_USERONCHANNEL(const String &nickname, const String &targetNick, const String &channelName)
	{
		return ":" + servername + " 443 " + nickname + " " + targetNick + " " + channelName + " :is already on channel";
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

	String ERR_CHANNELISFULL(const String &username, const String &channelName)
	{
		return ":" + servername + " 471 " + username + " " + channelName + " :Cannot join channel (+l)";
	}

	String ERR_INVITEONLYCHAN(const String &username, const String &channelName)
	{
		return ":" + servername + " 473 " + username + " " + channelName + " :Cannot join channel (+i)";
	}

	String ERR_BADCHANNELKEY(const String &username, const String &channelName)
	{
		return ":" + servername + " 475 " + username + " " + channelName + " :Cannot join channel (+k)";
	}

	String ERR_CHANOPRIVSNEEDED(const String &nickname, const String &channelName)
	{
		return ":" + servername + " 482 " + nickname + " " + channelName + " :You're not channel operator";
	}

	String RPL_SUCCNICK(const String &prefix, const String &newNick)
	{
		return ":" + prefix + " NICK :" + newNick;
	}

	String RPL_QUIT(const String &prefix, const String &msg)
	{
		return ":" + prefix + " QUIT :" + msg;
	}

	String RPL_JOIN(const String &prefix, const String &channelName)
	{
		return ":" + prefix + " JOIN :" + channelName;
	}

	String RPL_PART(const String &prefix, const String &channelName, const String &msg)
	{
		return ":" + prefix + " PART " + channelName + (msg.empty() ? "" : " :" + msg);
	}

	String RPL_SUCCINVITE(const String &prefix, const String &targetNick, const String &channelName)
	{
		return ":" + prefix + " INVITE " + targetNick + " :" + channelName;
	}

	String RPL_SUCCTOPIC(const String &prefix, const String &channelName, const String &topic)
	{
		return ":" + prefix + " TOPIC " + channelName + " :" + topic;
	}

	String RPL_SUCCKICK(const String &prefix, const String &channelName, const String &targetNick, const String &msg)
	{
		return ":" + prefix + " KICK " + channelName + " " + targetNick + (msg.empty() ? "" : " :" + msg);
	}

}
