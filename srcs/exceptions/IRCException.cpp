#include <IRCException.hpp>

IRCException::IRCException::IRCException(const String &message) : _msg(message) {}

IRCException::IRCException::~IRCException() throw() {}

const char *IRCException::IRCException::what() const throw()
{
	return _msg.c_str();
}
