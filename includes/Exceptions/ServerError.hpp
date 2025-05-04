#ifndef SERVERERROR_HPP
#define SERVERERROR_HPP

#include <IRCException.hpp>

namespace IRCException
{
	class ServerError : public IRCException
	{
	public:
		ServerError(const String &message);
	};
}

#endif
