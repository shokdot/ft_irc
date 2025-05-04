#include <ServerError.hpp>

IRCException::ServerError::ServerError(const String &message)
	: IRCException("Server Error: " + message)
{
}
