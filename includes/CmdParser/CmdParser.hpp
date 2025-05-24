#ifndef CMD_PARSER_HPP
#define CMD_PARSER_HPP

#include <IRC.hpp>

class CmdParser
{
public:
	static void parseCmd(String &rawLine);
	static bool isValidHostname(const String &hostname);
	static bool isValidShortName(const String &shortName);
	static std::vector<String> splitByDelim(const String &str, char delimeter);
	static bool isValidNick(const String &nick);
	static bool isValidHostAddr(const String &hostAddr);
	static bool isSpecial(char c);
	static bool isValidHostPart(const String &part);
	static bool isValidHost(const String &host);
	static bool isValidPrefix(const String &prefix);
	static bool isValidUser(const String &user);
};

#endif
