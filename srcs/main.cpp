#include <IRC.hpp>

int main(int ac, char **av)
{
	try
	{
		Server server(Parsing::args_validate(ac, av));
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
