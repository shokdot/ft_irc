#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <IRC.hpp>

namespace Reply
{
	String RPL_WELCOME(const String &nickname, const String &prefix);
	String RPL_YOURHOST(const String &nickname);
	String RPL_CREATED(const String &nickname);
	String RPL_MYINFO(const String &nickname);
	String ERR_UNKNOWNCOMMAND(const String &nickname, const String &cmd);
	String ERR_NONICKNAMEGIVEN(const String &nickname);
	String ERR_ERRONEUSNICKNAME(const String &oldNick, const String &newNick);
	String ERR_NICKNAMEINUSE(const String &oldNick, const String &newNick);
	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName);
	String ERR_ALREADYREGISTRED(const String &nickname);
	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason);
	String RPL_SUCCNICK(const String &prefix, const String &newNick);
	String RPL_QUIT(const String &prefix, const String &msg = "Client Quit");
}

#endif
