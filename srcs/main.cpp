#include <IRCServer.hpp>

int main(int ac, char **av)
{
	try
	{
		IRCServer server(Parsing::validateArgs(ac, av));
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	return 0;
}
