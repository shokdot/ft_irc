#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <IRC.hpp>

namespace Reply
{
	String RPL_WELCOME(const String &nickname, const String &prefix);
	String RPL_YOURHOST(const String &nickname);
	String RPL_CREATED(const String &nickname, const String &time);
	String RPL_MYINFO(const String &nickname);
	String RPL_NOTOPIC(const String &nickname, const String &channelName);
	String RPL_TOPIC(const String &nickname, const String &channelName, const String &topic);
	String RPL_NAMREPLY(const String &nickname, const String &channelName, const String &userList);
	String RPL_ENDOFNAMES(const String &nickname, const String &channelName);
	String ERR_NOSUCHCHANNEL(const String &nickname, const String &channelName);
	String ERR_UNKNOWNCOMMAND(const String &nickname, const String &cmd);
	String ERR_NONICKNAMEGIVEN(const String &nickname);
	String ERR_ERRONEUSNICKNAME(const String &oldNick, const String &newNick);
	String ERR_NICKNAMEINUSE(const String &oldNick, const String &newNick);
	String ERR_NOTONCHANNEL(const String &nickname, const String &channelName);
	String ERR_NOTREGISTERED(const String &nickname);
	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName);
	String ERR_ALREADYREGISTRED(const String &nickname);
	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason);
	String ERR_INVALIDUSERNAME(const String &username);
	String ERR_CHANNELISFULL(const String &username, const String &channelName);
	String ERR_INVITEONLYCHAN(const String &username, const String &channelName);
	String ERR_BADCHANNELKEY(const String &username, const String &channelName);
	String RPL_SUCCNICK(const String &prefix, const String &newNick);
	String RPL_QUIT(const String &prefix, const String &msg = "Client Quit");
	String RPL_JOIN(const String &prefix, const String &channelName);
	String RPL_PART(const String &prefix, const String &channelName, const String &msg);
}

#endif
