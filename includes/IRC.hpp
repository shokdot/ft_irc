#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_CONN 100

using std::cout;
using std::endl;
typedef std::string String;

String trim(const String &str);
void send_wrapper(const String &str, int sock_fd);

#include <Parsing.hpp>
#include <Server.hpp>
#include <User.hpp>
#include <Channel.hpp>

#endif
