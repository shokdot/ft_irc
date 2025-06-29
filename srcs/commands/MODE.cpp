#include <MODE.hpp>
#include <IRCServer.hpp>

MODE::MODE() {}

MODE::~MODE() {}

void MODE::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	const String &nickname = client->getNickname();
	ChannelManager &channelManager = server.getChannelManager();

	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NEEDMOREPARAMS(nickname, "MODE"));
		return;
	}

	String &channelName = cmd.params[0];
	Channel *channel = channelManager.getChannelByName(channelName);

	if (!channel)
	{
		client->sendReply(Reply::ERR_NOSUCHCHANNEL(nickname, channelName));
		return;
	}

	if (cmd.params.size() == 1)
	{
		client->sendReply(Reply::RPL_CHANNELMODEIS(nickname, channelName, channel->getModeString(), channel->getModeParams()));
		return;
	}

	if (!channel->isOperator(client))
	{
		client->sendReply(Reply::ERR_CHANOPRIVSNEEDED(nickname, channelName));
		return;
	}

	if (cmd.params[1].find('+') == cmd.params[1].find('-'))
	{
		client->sendReply(Reply::ERR_UNKNOWNMODE(nickname, cmd.params[1][0], channelName));
		return;
	}

	handleChannelMode(client, cmd, channel, server);
}

void MODE::handleChannelMode(Client *client, CmdStruct &cmd, Channel *channel, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	std::vector<ModeChange> parsedModes = parseModeRaw(cmd.params);
	std::vector<ModeChange> validModes = filterValidModes(client, parsedModes, channel, clientManager);

	for (std::size_t i = 0; i < validModes.size(); ++i)
	{
		ModeChange &cmd = validModes[i];

		if (cmd.action == '+')
			setMode(cmd, channel, clientManager);
		else
			unsetMode(cmd, channel, clientManager);
	}

	if (validModes.size() > 0)
	{
		String changes = getOperations(validModes);
		channel->broadcastToChannel(Reply::RPL_SUCCMODE(client->getPrefix(), channel->getName(), changes));
	}
}

String MODE::getOperations(const std::vector<ModeChange> &operations)
{
	std::ostringstream operationsString;
	std::vector<String> params;

	char currentAction = '\0';
	for (std::size_t i = 0; i < operations.size(); ++i)
	{
		const ModeChange &mc = operations[i];
		if (mc.action != currentAction)
		{
			currentAction = mc.action;
			operationsString << currentAction;
		}
		operationsString << mc.mode;
		if (!mc.param.empty())
		{
			params.push_back(mc.param);
		}
	}

	for (std::size_t i = 0; i < params.size(); ++i)
		operationsString << " " << params[i];

	return operationsString.str();
}

bool MODE::isValidMode(char c)
{
	static const std::string valid = "iotkl";
	return valid.find(c) != std::string::npos;
}

std::vector<ModeChange> MODE::filterValidModes(Client *client, const std::vector<ModeChange> &input, Channel *channel, ClientManager &clientManager)
{
	std::vector<ModeChange> valid;

	for (std::size_t i = 0; i < input.size(); ++i)
	{
		const ModeChange &mc = input[i];

		if (!isValidMode(mc.mode))
		{
			client->sendReply(Reply::ERR_UNKNOWNMODE(client->getNickname(), mc.mode, channel->getName()));
			continue;
		}

		if (isModeNeedParam(mc.mode, mc.action) && mc.param.empty())
			continue;

		if (mc.mode == 'k' && mc.action == '+')
		{
			if (!ChannelManager::isValidKey(mc.param))
				continue;
			if (channel->hasMode('k'))
			{
				client->sendReply(Reply::ERR_KEYSET(client->getNickname(), channel->getName()));
				continue;
			}
		}

		if (mc.mode == 'l' && mc.action == '+' && (!isValidNumber(mc.param) || std::atoi(mc.param.c_str()) < 0))
			continue;

		if (mc.mode == 'o')
		{
			Client *recipient = clientManager.getClientByNick(mc.param);
			if (!recipient || !channel->hasClient(recipient))
			{
				client->sendReply(Reply::ERR_NOTONCHANNEL(mc.param, channel->getName()));
				continue;
			}
		}

		valid.push_back(mc);
	}

	return valid;
}

std::vector<String> MODE::groupArgs(const std::vector<String> &args)
{
	std::vector<String> result;
	String current;

	for (std::size_t i = 0; i < args.size(); ++i)
	{
		const String &token = args[i];

		if (token == "+" || token == "-")
		{
			if (!current.empty())
				result.push_back(current);
			current = token;
		}
		else if (!current.empty() && token.size() == 1 && std::isalpha(token[0]))
		{
			current += token;
		}
		else
		{
			if (!current.empty())
			{
				result.push_back(current);
				current.clear();
			}
			result.push_back(token);
		}
	}

	if (!current.empty())
		result.push_back(current);

	return result;
}

bool MODE::isModeNeedParam(char mode, char action)
{
	if (mode == 'o')
		return true;
	if (mode == 'l' || mode == 'k')
		return action == '+';
	return false;
}

std::vector<ModeChange> MODE::parseModeRaw(const std::vector<String> rawInput)
{
	std::vector<ModeChange> result;
	std::vector<String> modeParams(rawInput.begin() + 1, rawInput.end());
	std::vector<String> args = groupArgs(modeParams);

	if (args.empty())
		return result;

	std::vector<String> modeBlocks;
	std::vector<String> params;

	for (std::size_t i = 0; i < args.size(); ++i)
	{
		const String &arg = args[i];
		if (!arg.empty() && (arg[0] == '+' || arg[0] == '-'))
			modeBlocks.push_back(arg);
		else
			params.push_back(arg);
	}

	std::size_t paramIndex = 0;
	char currentAction = 0;

	for (std::size_t i = 0; i < modeBlocks.size(); ++i)
	{
		const String &modeStr = modeBlocks[i];
		if (modeStr.empty())
			continue;

		for (std::size_t j = 0; j < modeStr.size(); ++j)
		{
			char c = modeStr[j];
			if (c == '+' || c == '-')
				currentAction = c;
			else
			{
				if (!std::isalpha(c))
					continue;

				ModeChange mc;
				mc.action = currentAction;
				mc.mode = c;

				if (isModeNeedParam(c, currentAction) && paramIndex < params.size())
					mc.param = params[paramIndex++];
				result.push_back(mc);
			}
		}
	}

	return result;
}

bool MODE::isValidNumber(const String &input)
{
	if (input.size() > 9)
		return false;
	for (std::size_t i = 0; i < input.size(); ++i)
		if (!std::isdigit(input[i]))
			return false;
	return true;
}

void MODE::setMode(const ModeChange &cmd, Channel *channel, ClientManager &clientManager)
{
	switch (cmd.mode)
	{
	case 'i':
		channel->addMode('i');
		break;
	case 'o':
		channel->addOperator(clientManager.getClientByNick(cmd.param));
		break;
	case 'k':
		channel->addMode('k');
		channel->setPassword(cmd.param);
		break;
	case 't':
		channel->addMode('t');
		break;
	case 'l':
		channel->addMode('l');
		channel->setUserLimit(std::atoi(cmd.param.c_str()));
		break;
	default:
		break;
	}
}

void MODE::unsetMode(const ModeChange &cmd, Channel *channel, ClientManager &clientManager)
{
	switch (cmd.mode)
	{
	case 'i':
		channel->removeMode('i');
		break;
	case 'o':
		channel->removeOperator(clientManager.getClientByNick(cmd.param));
		break;
	case 'k':
		channel->removeMode('k');
		channel->setPassword("");
		break;
	case 't':
		channel->removeMode('t');
		break;
	case 'l':
		channel->removeMode('l');
		channel->setUserLimit(0);
		break;
	default:
		break;
	}
}
