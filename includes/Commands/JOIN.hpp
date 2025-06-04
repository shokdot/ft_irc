#ifndef JOIN_HPP
#define JOIN_HPP

#include <ACommand.hpp>

class JOIN : public ACommand
{
public:
	JOIN();
	~JOIN();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);

private:
	std::vector<ChannelKey> parseChannels(Client *client, std::vector<String> &params);
	bool isValidChannelName(const String &name);
	bool isValidKey(const String &key);
};
#endif
