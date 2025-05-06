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

void Utils::sendWrapper(const String &str, int sock_fd)
{
	send(sock_fd, str.c_str(), str.length(), 0);
}
