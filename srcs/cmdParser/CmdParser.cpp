#include <CmdParser.hpp>

bool CmdParser::parseCmd(String &rawLine, CmdStruct &command)
{
	if (rawLine.empty() || rawLine.size() > 510)
		return false;
	if (rawLine[0] == ':' && !parsePrefix(rawLine))
		return false;
	if (!parseCmdName(rawLine, command.cmdName))
		return false;
	if (!parseParams(rawLine, command.cmdName, command.params, command.trailing))
		return false;
	return true;
}

bool CmdParser::parsePrefix(String &rawLine)
{
	size_t spacePos = rawLine.find(' ');
	if (spacePos == String::npos || spacePos == 1)
		return false;
	rawLine = rawLine.erase(0, spacePos + 1);
	return true;
}

bool CmdParser::parseCmdName(String &rawLine, String &cmdName)
{
	size_t spacePos = rawLine.find(' ');
	if (spacePos == String::npos)
	{
		cmdName = rawLine;
		rawLine.clear();
	}
	else
	{
		cmdName = rawLine.substr(0, spacePos);
		rawLine = rawLine.erase(0, spacePos + 1);
	}
	if (cmdName.empty())
		return false;
	char c;
	for (size_t i = 0; i < cmdName.size(); ++i)
	{
		c = static_cast<unsigned char>(cmdName[i]);
		if (!std::isalpha(c))
			return false;
	}
	cmdName = Utils::strToUpper(cmdName);
	return true;
}

bool CmdParser::parseParams(String &rawLine, String &cmdName, std::vector<String> &params, String &trailing)
{
	params.clear();
	trailing = "";
	std::istringstream iss(rawLine);
	std::string token;
	int paramCount = 0;

	while (iss >> token)
	{
		if (token[0] == ':' || paramCount == 14)
		{
			std::string tmp_trailing;
			if (token[0] == ':' && cmdName != "TOPIC")
				tmp_trailing = token.substr(1);
			else
				tmp_trailing = token;

			std::string rest;
			std::getline(iss, rest);
			if (!rest.empty())
				tmp_trailing += rest;

			trailing = tmp_trailing;
			break;
		}
		else
		{
			params.push_back(token);
			paramCount++;
		}
	}
	for (size_t i = 0; i < params.size(); i++)
	{
		if (!isValidMiddle(params[i]))
			return false;
	}
	if (!isValidTrailing(trailing))
		return false;

	return true;
}

bool CmdParser::isValidTrailing(const String &trailing)
{
	char c;
	for (size_t i = 0; i < trailing.size(); i++)
	{
		c = static_cast<unsigned char>(trailing[i]);
		if (c != ':' && c != ' ' && !isNospcrlfcl(c))
			return false;
	}

	return true;
}

bool CmdParser::isValidMiddle(const String &middle)
{
	if (middle.empty())
		return false;
	if (!isNospcrlfcl(middle[0]))
		return false;
	char c;
	for (size_t i = 1; i < middle.size(); ++i)
	{
		c = static_cast<unsigned char>(middle[i]);
		if (c != ':' && !isNospcrlfcl(c))
			return false;
	}
	return true;
}

bool CmdParser::isNospcrlfcl(const char c)
{
	unsigned char uc = static_cast<unsigned char>(c);
	return (uc >= 1 && uc <= 9) ||
		   (uc >= 11 && uc <= 12) ||
		   (uc >= 14 && uc <= 31) ||
		   (uc >= 33 && uc <= 57) ||
		   (uc >= 58 && uc <= 255);
}
