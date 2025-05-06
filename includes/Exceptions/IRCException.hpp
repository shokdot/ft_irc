#ifndef IRCEXCEPTION_HPP
#define IRCEXCEPTION_HPP

#include <IRC.hpp>

namespace IRCException
{
	class IRCException : public std::exception
	{
	public:
		IRCException(const String &message);
		virtual ~IRCException() throw();

		virtual const char *what() const throw();

	private:
		String _msg;
	};
}

#include <InputError.hpp>
#include <ServerError.hpp>

#endif
// IRCEXCEPTION_HPP
