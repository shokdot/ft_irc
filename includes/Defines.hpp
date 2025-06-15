#ifndef DEFINES_HPP
#define DEFINES_HPP

#define implements public

enum Defines
{
	MAX_CONN = 100,
	PORT_MIN_RANGE = 1024,
	PORT_MAX_RANGE = 65535
};

enum PollState
{
	MESSAGE,
	ERROR,
	NEWCONN
};

struct CmdStruct
{
	String cmdName;
	std::vector<String> params;
	String trailing;
};

struct ChannelKey
{
	String channel;
	String key;

	ChannelKey(const String &ch, const String &k) : channel(ch), key(k) {}
};

#endif
// DEFINES_HPP
