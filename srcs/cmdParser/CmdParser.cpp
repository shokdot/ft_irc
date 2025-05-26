#include <CmdParser.hpp>

bool CmdParser::parseCmd(String &rawLine, FullCmd &command)
{
	if (rawLine.empty() || rawLine.size() > 510)
		return false;
	if (rawLine.front() == ':' && !parsePrefix(rawLine, command.prefix))
		return false;
	if (!parseCmdName(rawLine, command.cmdName))
		return false;

	return true;
}

bool CmdParser::parseCmdName(String &rawLine, String &cmdName)
{
	size_t spacePos = rawLine.find(' ');
	if (spacePos == String::npos)
		cmdName = rawLine;
	else
	{
		cmdName = rawLine.substr(0, spacePos);
		rawLine = rawLine.erase(0, spacePos + 1);
	}
	if (cmdName.empty())
		return false;
	int c;
	for (size_t i = 0; i < cmdName.size(); ++i)
	{
		c = static_cast<unsigned char>(cmdName[i]);
		if (!std::isalpha(c))
			return false;
	}
	cmdName = Utils::strToUpper(cmdName);
	return true;
}

bool CmdParser::parsePrefix(String &rawLine, CmdPrefix &cmdPrefix)
{
	size_t spacePos = rawLine.find(' ');
	if (spacePos == String::npos || spacePos == 1)
		return false;
	String prefix = rawLine.substr(1, spacePos - 1);
	if (!extractPrefix(prefix, cmdPrefix))
		return false;
	rawLine = rawLine.erase(0, spacePos + 1);
	return true;
}

bool CmdParser::isValidHostname(const String &hostname)
{
	if (hostname.empty())
		return false;

	std::vector<String> parts = splitByDelim(hostname, '.');
	for (size_t i = 0; i < parts.size(); ++i)
		if (!isValidShortName(parts[i]))
			return false;
	return true;
}

bool CmdParser::isValidShortName(const String &shortName)
{
	if (shortName.empty() || shortName.size() > 63)
		return false;
	if (!std::isalnum(static_cast<unsigned char>(shortName.front())))
		return false;
	if (!std::isalnum(static_cast<unsigned char>(shortName.back())))
		return false;

	bool lastWasHyphen = false;
	int c;
	for (size_t i = 0; i < shortName.size(); ++i)
	{
		c = static_cast<unsigned char>(shortName[i]);
		if (!std::isalnum(c) && c != '-')
			return false;
		if (c == '-')
		{
			if (lastWasHyphen)
				return false;
			lastWasHyphen = true;
		}
		else
			lastWasHyphen = false;
	}
	return true;
}

std::vector<String> CmdParser::splitByDelim(const String &str, char delimiter)
{
	std::vector<String> parts;
	std::stringstream ss(str);
	String item;
	while (std::getline(ss, item, delimiter))
		parts.push_back(item);
	return parts;
}

bool CmdParser::isValidNick(const String &nick)
{
	if (nick.empty() || nick.size() > 9)
		return false;
	if (!std::isalpha(static_cast<unsigned char>(nick.front())) && !isSpecial(static_cast<unsigned char>(nick.front())))
		return false;
	char c;
	for (size_t i = 0; i < nick.size(); ++i)
	{
		c = static_cast<unsigned char>(nick[i]);
		if (!std::isalnum(c) && !isSpecial(c) && c == '-')
			return false;
	}
	return true;
}

bool CmdParser::isSpecial(char c)
{
	return (c >= '[' && c <= '`') || (c >= '{' && c <= '}');
}

bool CmdParser::isValidHostAddr(const String &hostAddr)
{
	std::vector<String> hostPart = splitByDelim(hostAddr, '.');
	if (hostPart.size() != 4)
		return false;
	for (size_t i = 0; i < hostPart.size(); ++i)
	{
		if (!isValidHostPart(hostPart[i]))
			return false;
	}
	return true;
}

bool CmdParser::isValidHostPart(const String &part)
{
	if (part.empty() || part.size() > 3)
		return false;
	char c;
	for (size_t i = 0; i < part.size(); ++i)
	{
		c = static_cast<unsigned char>(part[i]);
		if (!std::isdigit(c))
			return false;
	}
	int num = std::stoi(part);
	return num >= 0 && num <= 255;
}

bool CmdParser::isValidHost(const String &host)
{
	if (!isValidHostname(host) && !isValidHostAddr(host))
		return false;
	return true;
}

bool CmdParser::extractPrefix(const String &prefix, CmdPrefix &res)
{
	size_t exclPos = prefix.find('!');
	size_t atPos = prefix.find('@');

	if (exclPos != String::npos && atPos != String::npos && exclPos < atPos)
	{
		res.nickname = prefix.substr(0, exclPos);
		res.user = prefix.substr(exclPos + 1, atPos - exclPos - 1);
		res.host = prefix.substr(atPos + 1);
		res.type = PREFIX_USER;

		if (!isValidNick(res.nickname) || !isValidUser(res.user) || !isValidHost(res.host))
			return false;
	}
	else if (atPos != String::npos)
	{
		res.nickname = prefix.substr(0, atPos);
		res.host = prefix.substr(atPos + 1);
		res.user = "";
		res.type = PREFIX_USER;

		if (!isValidNick(res.nickname) || !isValidHost(res.host))
			return false;
	}
	else
	{
		if (isValidNick(prefix))
		{
			res.nickname = prefix;
			res.user = "";
			res.host = "";
			res.type = PREFIX_USER;
		}
		else if (isValidHostname(prefix))
		{
			res.server = prefix;
			res.type = PREFIX_SERVER;
		}
		else
			return false;
	}

	return true;
}

bool CmdParser::isValidUser(const String &user)
{
	if (user.empty())
		return false;
	for (size_t i = 0; i < user.size(); ++i)
	{
		char c = user[i];
		if (c == ' ' || c == '@' || c == '!' || c == '\r' || c == '\n' || c == '\0')
			return false;
	}
	return true;
}
