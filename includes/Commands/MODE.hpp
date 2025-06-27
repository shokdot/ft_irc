#ifndef MODE_HPP
#define MODE_HPP

#include <ACommand.hpp>

class MODE : public ACommand
{
public:
	MODE();
	~MODE();
	void execute(Client *client, CmdStruct &cmd, IRCServer &server);

private:
	void handleChannelMode(Client *client, CmdStruct &cmd, Channel *channel, IRCServer &server);
	bool isModeNeedParam(char mode, char action);
	std::vector<ModeChange> parseModeRaw(const std::vector<String> rawInput);
	std::vector<String> groupArgs(const std::vector<String> &args);
	std::vector<ModeChange> filterValidModes(Client *client, const std::vector<ModeChange> &input, Channel *channel, ClientManager &clientManager);
	bool isValidMode(char c);
	bool isValidNumber(const String &input);
	void setMode(const ModeChange &cmd, Channel *channel, ClientManager &clientManager);
	void unsetMode(const ModeChange &cmd, Channel *channel, ClientManager &clientManager);
	String getOperations(const std::vector<ModeChange> &operations);
};
#endif
