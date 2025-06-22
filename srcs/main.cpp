#include <IRCServer.hpp>

int main(int ac, char **av)
{
	try
	{
		std::signal(SIGINT, Utils::handleSignal);
		std::signal(SIGPIPE, SIG_IGN);
		std::pair<int, String> conf = InputValidator::validateArgs(ac, av);
		IRCServer server(conf.first, conf.second);
		server.start();
		server.stop();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
