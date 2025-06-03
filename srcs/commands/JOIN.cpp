#include <JOIN.hpp>
#include <IRCServer.hpp>

JOIN::JOIN()
{
}

JOIN::~JOIN()
{
}

void JOIN::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	(void)server;
	(void)cmd;
	(void)client;
	// int fd = client->getClientFd();
	// if (cmd.params.empty())
	// {
	// 	std::string reply = ":localhost 461 USER :Not enough parameters\r\n";
	// 	Utils::sendWrapper(reply, fd);
	// 	return;
	// }
	// std::vector<String> channels = Utils::splitByDelim(cmd.params[0], ',');
	// std::vector<String> keys;
	// if (cmd.params.size() > 1)
	// 	keys = Utils::splitByDelim(cmd.params[1], ',');

	// for (size_t i = 0; i < channels.size(); i++)
	// {
	// 	std::cout << channels[i] << std::endl;
	// }
	// for (size_t i = 0; i < keys.size(); ++i)
	// {
	// 	std::cout << keys[i] << std::endl;
	// }
	// std::cout << keys.size() << std::endl;

	// (void)server;
}
