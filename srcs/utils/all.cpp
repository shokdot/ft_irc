#include <IRC.hpp>

String rtrim(const String &str)
{
	std::string::size_type end = str.find_last_not_of(" \t\n\r\f\v");
	if (end == std::string::npos)
	{
		return "";
	}
	return str.substr(0, end + 1);
}

String ltrim(const String &str)
{
	std::string::size_type start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
	{
		return "";
	}
	return str.substr(start);
}

String trim(const String &str)
{
	return ltrim(rtrim(str));
}

void send_wrapper(const String &str, int sock_fd)
{
	send(sock_fd, str.c_str(), str.length(), 0);
}
