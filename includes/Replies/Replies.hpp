#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <IRC.hpp>

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
}

#endif
