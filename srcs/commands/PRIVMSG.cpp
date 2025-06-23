#include <PRIVMSG.hpp>
#include <IRCServer.hpp>

PRIVMSG::PRIVMSG()
{
}

PRIVMSG::~PRIVMSG()
{
}

void PRIVMSG::execute(Client *client, CmdStruct &cmd, IRCServer &server)
{
	if (!client)
		return;

	ClientManager &clientManager = server.getClientManager();

	String nickname = client->getNickname();

	if (cmd.params.empty())
	{
		client->sendReply(Reply::ERR_NORECIPIENT(nickname, "PRIVMSG"));
		return;
	}
	if (cmd.trailing.empty())
	{
		client->sendReply(Reply::ERR_NOTEXTTOSEND(nickname));
		return;
	}

	if (cmd.trailing.find("\x01"
						  "DCC SEND") != String::npos)
	{
		Client *recipent = clientManager.getClientByNick(cmd.params[0]);
		if (!recipent)
			return;
		sendFile(client, recipent, cmd.trailing);
		return;
	}

	if (cmd.params[0] == "bot")
	{
		botHandle(client, cmd.trailing);
		return;
	}

	std::vector<String> targets = Utils::splitByDelim(cmd.params[0], ',');
	if (targets.size() > 20)
	{
		client->sendReply(Reply::ERR_TOOMANYTARGETS(nickname, cmd.params[0]));
		return;
	}
	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (!handleTarget(client, targets[i], cmd.trailing, server))
			continue;
	}
}

bool PRIVMSG::handleTarget(Client *client, const String &target, const String &msg, IRCServer &server)
{
	ClientManager &clientManager = server.getClientManager();
	ChannelManager &channelManager = server.getChannelManager();
	String nickname = client->getNickname();

	if (ChannelManager::isValidChannelName(target))
	{
		Channel *channel = channelManager.getChannelByName(target);
		if (!channel)
		{
			client->sendReply(Reply::ERR_NOSUCHNICK(nickname, target));
			return false;
		}
		if (!channel->hasClient(client))
		{
			client->sendReply(Reply::ERR_CANNOTSENDTOCHAN(nickname, target));
			return false;
		}
		channel->broadcastToChannel(Reply::RPL_PRIVMSG(client->getPrefix(), target, msg), client->getClientFd());
	}
	else
	{
		Client *targetClient = clientManager.getClientByNick(target);
		if (!targetClient)
		{
			client->sendReply(Reply::ERR_NOSUCHNICK(nickname, target));
			return false;
		}
		targetClient->sendReply(Reply::RPL_PRIVMSG(client->getPrefix(), target, msg));
	}
	return true;
}

void PRIVMSG::sendFile(Client *client, Client *recipent, const String &msg)
{
	recipent->sendReply(Reply::RPL_PRIVMSG(client->getPrefix(), recipent->getNickname(), msg));
	return;
}

void PRIVMSG::botHandle(Client *client, const String &msg)
{
	String answer;
	if (msg == "hello" || msg == "hi" || msg == "hey")
	{
		answer = "Hello, welcome to the irc.42.chat!";
	}
	else if (msg == "bye" || msg == "goodbye")
	{
		answer = "Goodbye! See you next time.";
	}
	else if (msg == "help" || msg == "what can you do?")
	{
		answer = "I can chat with you! Try saying 'hello', 'joke', 'bye', or 'time'.";
	}
	else if (msg == "joke")
	{
		answer = "Why do programmers prefer dark mode? Because light attracts bugs!";
	}
	else if (msg == "time")
	{
		answer = "I'm just a bot, but maybe check your system clock.";
	}
	else if (msg == "thanks" || msg == "thank you")
	{
		answer = "You're welcome!";
	}
	else if (msg == "who are you")
	{
		answer = "I'm a simple IRC bot, here to make your day better.";
	}
	else
	{
		answer = "I'm not sure how to respond to that. Try 'help'.";
	}
	client->sendReply(Reply::RPL_PRIVMSG("bot!bigbot@localhost", client->getNickname(), answer));
	return;
}
