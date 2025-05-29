#include <CmdParser.hpp>

bool CmdParser::parseCmd(String &rawLine, CmdStruct &command)
{
	if (rawLine.empty() || rawLine.size() > 510)
		return false;
	if (rawLine[0] == ':' && !parsePrefix(rawLine))
		return false;
	if (!parseCmdName(rawLine, command.cmdName))
		return false;
	if (!parseParams(rawLine, command.params, command.trailing))
		return false;
	return true;
}

bool CmdParser::parsePrefix(String &rawLine)
{
	size_t spacePos = rawLine.find(' ');
	if (spacePos == String::npos || spacePos == 1)
		return false;
	// String prefix = rawLine.substr(1, spacePos - 1);
	// if (!extractPrefix(prefix, cmdPrefix))
	// 	return false;
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

bool CmdParser::parseParams(String &rawLine, std::vector<String> &params, String &trailing)
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
			if (token[0] == ':')
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

// bool CmdParser::isValidHostname(const String &hostname) // fix
// {
// 	if (hostname.empty() || hostname.length() > 253)
// 		return false;

// 	std::vector<String> parts = splitByDelim(hostname, '.');
// 	for (size_t i = 0; i < parts.size(); ++i)
// 		if (!isValidShortName(parts[i]))
// 			return false;
// 	return true;
// }

// bool CmdParser::isValidShortName(const String &shortName)
// {
// 	if (shortName.empty() || shortName.size() > 63)
// 		return false;
// 	if (!std::isalnum(static_cast<unsigned char>(shortName.front())))
// 		return false;
// 	if (!std::isalnum(static_cast<unsigned char>(shortName.back())))
// 		return false;

// 	char c;
// 	for (size_t i = 0; i < shortName.size(); ++i)
// 	{
// 		c = static_cast<unsigned char>(shortName[i]);
// 		if (!(std::isalnum(c) || c == '-'))
// 			return false;
// 	}
// 	return true;
// }

// std::vector<String> CmdParser::splitByDelim(const String &str, char delimiter)
// {
// 	std::vector<String> parts;
// 	std::stringstream ss(str);
// 	String item;
// 	while (std::getline(ss, item, delimiter))
// 		parts.push_back(item);
// 	return parts;
// }

// bool CmdParser::isValidNick(const String &nick)
// {
// 	if (nick.empty() || nick.size() > 9)
// 		return false;
// 	if (!(std::isalpha(static_cast<unsigned char>(nick.front())) || isSpecial(static_cast<unsigned char>(nick.front()))))
// 		return false;
// 	char c;
// 	for (size_t i = 0; i < nick.size(); ++i)
// 	{
// 		c = static_cast<unsigned char>(nick[i]);
// 		if (!(std::isalnum(c) || isSpecial(c) || c == '-'))
// 			return false;
// 	}
// 	return true;
// }

// bool CmdParser::isSpecial(char c) // change it
// {
// 	return (c >= '[' && c <= '`') || (c >= '{' && c <= '}');
// }

// bool CmdParser::isValidHostAddr(const String &hostAddr) // fix
// {
// 	std::vector<String> hostPart = splitByDelim(hostAddr, '.');
// 	if (hostPart.size() != 4)
// 		return false;
// 	for (size_t i = 0; i < hostPart.size(); ++i)
// 	{
// 		if (!isValidHostPart(hostPart[i]))
// 			return false;
// 	}
// 	return true;
// }

// bool CmdParser::isValidHostPart(const String &part) // fix
// {
// 	if (part.empty() || part.size() > 3)
// 		return false;
// 	if (part.size() > 1 && part[0] == '0')
// 		return false;
// 	char c;
// 	for (size_t i = 0; i < part.size(); ++i)
// 	{
// 		c = static_cast<unsigned char>(part[i]);
// 		if (!std::isdigit(c))
// 			return false;
// 	}
// 	int num = std::stoi(part);
// 	return num >= 0 && num <= 255;
// }

// bool CmdParser::isValidHost(const String &host) // fix
// {
// 	return isValidHostAddr(host) || isValidHostname(host);
// 	// if (!isValidHostAddr(host) && !isValidHostname(host))
// 	// 	return false;
// 	// return true;
// }

// bool CmdParser::extractPrefix(const String &prefix, CmdPrefix &res)
// {
// 	size_t exclPos = prefix.find('!');
// 	size_t atPos = prefix.find('@');

// 	if (exclPos != String::npos && atPos != String::npos && exclPos < atPos)
// 	{
// 		res.nickname = prefix.substr(0, exclPos);
// 		res.user = prefix.substr(exclPos + 1, atPos - exclPos - 1);
// 		res.host = prefix.substr(atPos + 1);
// 		res.type = PREFIX_USER;

// 		if (!isValidNick(res.nickname) || !isValidUser(res.user) || !isValidHost(res.host))
// 			return false;
// 	}
// 	else if (atPos != String::npos)
// 	{
// 		res.nickname = prefix.substr(0, atPos);
// 		res.host = prefix.substr(atPos + 1);
// 		res.user = "";
// 		res.type = PREFIX_USER;

// 		if (!isValidNick(res.nickname) || !isValidHost(res.host))
// 			return false;
// 	}
// 	else
// 	{
// 		if (isValidNick(prefix))
// 		{
// 			res.nickname = prefix;
// 			res.user = "";
// 			res.host = "";
// 			res.type = PREFIX_USER;
// 		}
// 		else if (isValidHostname(prefix))
// 		{
// 			res.server = prefix;
// 			res.type = PREFIX_SERVER;
// 		}
// 		else
// 			return false;
// 	}

// 	return true;
// }

// bool CmdParser::isValidUserChar(char c)
// {
// 	c = static_cast<unsigned char>(c);
// 	return (c != '\0' && c != '\n' && c != '\r' && c != ' ' && c != '@');
// }

// bool CmdParser::isValidUser(const String &user)
// {
// 	if (user.empty())
// 		return false;
// 	for (size_t i = 0; i < user.size(); ++i)
// 	{
// 		char c = user[i];
// 		if (!isValidUserChar(c))
// 			return false;
// 	}
// 	return true;
// }
