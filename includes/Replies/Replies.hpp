#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <IRC.hpp>

namespace Reply
{
	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName);
	String ERR_ALREADYREGISTRED(const String &nickname);
	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason);
	String ERR_NONICKNAMEGIVEN(const String &nickname);
	String ERR_ERRONEUSNICKNAME(const String &oldNick, const String &newNick);
	String ERR_NICKNAMEINUSE(const String &oldNick, const String &newNick);
	String RPL_SUCCNICK(const String &prefix, const String &newNick);
}

#endif
