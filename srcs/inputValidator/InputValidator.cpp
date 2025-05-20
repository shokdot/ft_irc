#include <InputValidator.hpp>

std::pair<int, String> InputValidator::validateArgs(int ac, char *av[])
{
	if (ac != 3)
		throw IRCException::InputError("Arguments too few or too many");
	int port = validatePort(av[1]);
	String password = validatePassword(av[2]);
	return std::make_pair(port, password);
}

int InputValidator::validatePort(char *str)
{
	for (int i = 0; str[i]; ++i)
		if (!std::isdigit(str[i]))
			throw IRCException::InputError("Port must be a number");
	errno = 0;
	long port = std::strtol(str, 0, 10);
	if (errno == ERANGE || port > INT_MAX || port < INT_MIN)
		throw IRCException::InputError("Port must be between 1024 and 65535");
	else if (port < PORT_MIN_RANGE || port > PORT_MAX_RANGE)
		throw IRCException::InputError("Port must be between 1024 and 65535");
	return static_cast<int>(port);
}

String InputValidator::validatePassword(char *str)
{
	String password = String(str);
	if (password.length() < 8)
		throw IRCException::InputError("Password must be at least 8 characters long");
	return password;
}
