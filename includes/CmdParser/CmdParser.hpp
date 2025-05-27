#ifndef CMD_PARSER_HPP
#define CMD_PARSER_HPP

#include <IRC.hpp>

class CmdParser
{
public:
	static bool parseCmd(String &rawLine, FullCmd &command);
	static bool parsePrefix(String &rawLine, CmdPrefix &cmdPrefix);
	static bool extractPrefix(const String &prefix, CmdPrefix &res);
	static bool isValidHostname(const String &hostname);
	static bool isValidShortName(const String &shortName);
	static std::vector<String> splitByDelim(const String &str, char delimeter);
	static bool isValidNick(const String &nick);
	static bool isValidHostAddr(const String &hostAddr);
	static bool isSpecial(char c);
	static bool isValidHostPart(const String &part);
	static bool isValidHost(const String &host);
	static bool isValidUser(const String &user);
	static bool parseCmdName(String &rawLine, String &cmdName);
	static bool parseParams(String &rawLine, std::vector<String> &params, String &trailing);
	static bool isValidMiddle(const String &middle);
	static bool isNospcrlfcl(const char c);
	static bool isValidTrailing(const String &trailing);
};

#endif
