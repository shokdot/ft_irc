#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <Defines.hpp>
#include <exception>

using std::cout;
using std::endl;
typedef std::string String;

#include <Parsing.hpp>
#include <IRCServer.hpp>
#include <User.hpp>
#include <EventHandler.hpp>
#include <Channel.hpp>
#include <Utils.hpp>
#include <IRCException.hpp>

#endif
// IRC_HPP
