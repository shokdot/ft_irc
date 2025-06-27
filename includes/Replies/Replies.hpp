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
	String RPL_CHANNELMODEIS(const String &nickname, const String &channelName, const String &modes, const String &modeParams);
	String RPL_INVITING(const String &nickname, const String &targetNick, const String &channelName);
	String RPL_NAMREPLY(const String &nickname, const String &channelName, const String &userList);
	String RPL_ENDOFNAMES(const String &nickname, const String &channelName);
	String ERR_NOSUCHNICK(const String &nickname, const String &targetNick);
	String ERR_NOSUCHCHANNEL(const String &nickname, const String &channelName);
	String ERR_CANNOTSENDTOCHAN(const String &nickname, const String &channelName);
	String ERR_TOOMANYTARGETS(const String &nickname, const String &targetList);
	String ERR_NOORIGIN(const String &nickname);
	String ERR_NORECIPIENT(const String &nickname, const String &cmdName);
	String ERR_NOTEXTTOSEND(const String &nickname);
	String ERR_UNKNOWNCOMMAND(const String &nickname, const String &cmd);
	String ERR_NONICKNAMEGIVEN(const String &nickname);
	String ERR_ERRONEUSNICKNAME(const String &oldNick, const String &newNick);
	String ERR_NICKNAMEINUSE(const String &oldNick, const String &newNick);
	String ERR_USERNOTINCHANNEL(const String &nickname, const String &channelName);
	String ERR_NOTONCHANNEL(const String &nickname, const String &channelName);
	String ERR_USERONCHANNEL(const String &nickname, const String &targetNick, const String &channelName);
	String ERR_NOTREGISTERED(const String &nickname);
	String ERR_NEEDMOREPARAMS(const String &nickname, const String &cmdName);
	String ERR_ALREADYREGISTRED(const String &nickname);
	String ERR_PASSWDMISMATCH(const String &nickname, const String &reason);
	String ERR_KEYSET(const String &nickname, const String &channelName);
	String ERR_INVALIDUSERNAME(const String &username);
	String ERR_CHANNELISFULL(const String &nickname, const String &channelName);
	String ERR_UNKNOWNMODE(const String &nickname, const char mode, const String &channelName);
	String ERR_INVITEONLYCHAN(const String &nickname, const String &channelName);
	String ERR_BADCHANNELKEY(const String &nickname, const String &channelName);
	String ERR_BADCHANMASK(const String &nickname, const String &channelName);
	String ERR_CHANOPRIVSNEEDED(const String &nickname, const String &channelName);
	String RPL_SUCCNICK(const String &prefix, const String &newNick);
	String RPL_QUIT(const String &prefix, const String &msg = "Client Quit");
	String RPL_JOIN(const String &prefix, const String &channelName);
	String RPL_PART(const String &prefix, const String &channelName, const String &msg);
	String RPL_SUCCINVITE(const String &prefix, const String &targetNick, const String &channelName);
	String RPL_SUCCTOPIC(const String &prefix, const String &channelName, const String &topic);
	String RPL_SUCCKICK(const String &prefix, const String &channelName, const String &targetNick, const String &msg);
	String RPL_PRIVMSG(const String &prefix, const String &target, const String &msg);
	String RPL_SUCCMODE(const String &prefix, const String &channelName, const String &changes);
	String RPL_CAP();
	String RPL_PONG(const String &msg);
}

#endif
