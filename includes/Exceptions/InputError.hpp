#ifndef INPUTERROR_HPP
#define INPUTERROR_HPP

#include <IRCException.hpp>

namespace IRCException
{
	class InputError : public IRCException
	{
	public:
		InputError(const String &message);
	};
}

#endif
