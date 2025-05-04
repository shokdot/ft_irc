#include <InputError.hpp>

IRCException::InputError::InputError(const String &message)
	: IRCException("Input Error: " + message)
{
}
