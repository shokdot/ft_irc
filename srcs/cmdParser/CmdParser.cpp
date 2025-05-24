#include <CmdParser.hpp>

void CmdParser::parseCmd(String &rawLine)
{
	(void)rawLine;
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
	if (nick.empty() || nick.size() > 8)
		return false;
	if (!std::isalpha(static_cast<unsigned char>(nick.front())) || !isSpecial(static_cast<unsigned char>(nick.front())))
		return false;
	char c;
	for (size_t i = 0; i < nick.size(); ++i)
	{
		c = static_cast<unsigned char>(nick[i]);
		if (!std::isalnum(c) || !isSpecial(c) || c == '-')
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
	if (!isValidHostname(host) || !isValidHostAddr(host))
		return false;
	return true;
}

struct PrefixInfo
{
	std::string nickname;
	std::string user;
	std::string host;
};

bool CmdParser::isValidPrefix(const String &prefix)
{
	PrefixInfo result;
	size_t excl_pos = prefix.find('!');
	size_t at_pos = prefix.find('@');

	if (excl_pos != std::string::npos && at_pos != std::string::npos && excl_pos < at_pos)
	{
		result.nickname = prefix.substr(0, excl_pos);
		result.user = prefix.substr(excl_pos + 1, at_pos - excl_pos - 1);
		result.host = prefix.substr(at_pos + 1);
		if (!isValidNick(result.nickname) || !isValidUser(result.user) || !isValidHost(result.host))
			return false;
	}
	else if (at_pos != std::string::npos)
	{
		result.nickname = prefix.substr(0, at_pos);
		result.user = "";
		result.host = prefix.substr(at_pos + 1);
		if (!isValidNick(result.nickname) || !isValidHost(result.host))
			return false;
	}
	else
	{
		result.nickname = prefix;
		result.user = "";
		result.host = "";
		if (!isValidNick(result.nickname))
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
