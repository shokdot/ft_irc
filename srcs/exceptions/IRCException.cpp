#include <IRCException.hpp>

IRCException::IRCException::IRCException(const String &message) : m_msg(message) {}

IRCException::IRCException::~IRCException() throw() {}

const char *IRCException::IRCException::what() const throw()
{
	return m_msg.c_str();
}
