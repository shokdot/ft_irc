#ifndef CMD_PARSER_HPP
#define CMD_PARSER_HPP

#include <IRC.hpp>

class CmdParser
{
public:
	static bool parseCmd(String &rawLine, CmdStruct &command);

private:
	static bool parsePrefix(String &rawLine);
	static bool parseCmdName(String &rawLine, String &cmdName);
	static bool parseParams(String &rawLine, String &cmdName, std::vector<String> &params, String &trailing);
	static bool isValidTrailing(const String &trailing);
	static bool isValidMiddle(const String &middle);
	static bool isNospcrlfcl(const char c);
};

#endif
