#include <Parsing.hpp>

std::pair<int, String> Parsing::validateArgs(int ac, char *av[])
{
	if (ac != 3)
		throw IRCException::InputError("Arguments too few or too many");
	int port = validatePort(av[1]);
	String password = validatePassword(av[2]);
	return std::make_pair(port, password);
}

int Parsing::validatePort(char *str)
{
	for (int i = 0; str[i]; ++i)
		if (!std::isdigit(str[i]))
			throw IRCException::InputError("Port must be a number");
	errno = 0;
	long port = std::strtol(str, 0, 10);
	if (errno == ERANGE || port > INT_MAX || port < INT_MIN)
		throw IRCException::InputError("Port value out of int range");
	else if (port < 1024 || port > 65535)
		throw IRCException::InputError("Port value out of int range");
	return static_cast<int>(port);
}

String Parsing::validatePassword(char *str)
{
	String password = String(str);
	if (password.length() < 8)
		throw IRCException::InputError("Password must be at least 8 characters long");
	return password;
}
