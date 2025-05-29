#include <Utils.hpp>

String Utils::rtrim(const String &str)
{
	std::string::size_type end = str.find_last_not_of(" \t\n\r\f\v");
	if (end == std::string::npos)
	{
		return "";
	}
	return str.substr(0, end + 1);
}

String Utils::ltrim(const String &str)
{
	std::string::size_type start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
	{
		return "";
	}
	return str.substr(start);
}

String Utils::trim(const String &str)
{
	return Utils::ltrim(Utils::rtrim(str));
}

String Utils::strToUpper(const String &str)
{
	String res;
	res.reserve(str.size());
	for (size_t i = 0; i < str.size(); i++)
	{
		res += std::toupper(static_cast<unsigned char>(str[i]));
	}
	return res;
}

String Utils::strToLower(const String &str)
{
	String res;
	res.reserve(str.size());
	for (size_t i = 0; i < str.size(); i++)
	{
		res += std::tolower(static_cast<unsigned char>(str[i]));
	}
	return res;
}

void Utils::sendWrapper(const String &str, int sock_fd)
{
	send(sock_fd, str.c_str(), str.length(), 0);
}

struct sockaddr_in Utils::createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr)
{
	struct sockaddr_in sockStruct;
	sockStruct.sin_family = family;
	sockStruct.sin_port = htons(port);
	sockStruct.sin_addr.s_addr = addr;
	return sockStruct;
}

struct pollfd Utils::createPollStruct(int fd, short events)
{
	struct pollfd pollStruct;
	pollStruct.fd = fd;
	pollStruct.events = events;
	pollStruct.revents = 0;
	return pollStruct;
}
