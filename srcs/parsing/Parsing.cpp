#include <Parsing.hpp>

std::pair<int, String> Parsing::args_validate(int ac, char *av[])
{
	if (ac != 3)
		throw std::invalid_argument("Argument too much."); // custom exception
	int port = validate_port(av[1]);
	String password = validate_password(av[2]);
	return std::make_pair(port, password);
}

int Parsing::validate_port(char *str)
{
	for (int i = 0; str[i]; ++i)
		if (!std::isdigit(str[i]))
			throw std::invalid_argument("port isn't numeric"); // custom exception
	errno = 0;
	long port = std::strtol(str, 0, 10);
	if (errno == ERANGE || port > INT_MAX || port < INT_MIN)
		throw std::out_of_range("Port value out of int range");
	else if (port < 1024 || port > 65535)
		throw std::out_of_range("Port value out of int range");
	return static_cast<int>(port);
}

String Parsing::validate_password(char *str)
{
	String password = String(str);
	if (password.length() < 8)
		throw std::invalid_argument("password lenght mus't be at least 8 characters"); // custom exception
	return password;
}
