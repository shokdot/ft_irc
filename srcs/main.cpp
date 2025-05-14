#include <IRCServer.hpp>

int main(int ac, char **av)
{
	try
	{
		std::pair<int, String> conf = InputValidator::validateArgs(ac, av);
		IRCServer server(conf.first, conf.second);
		server.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	return 0;
}
