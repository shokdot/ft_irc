#ifndef IRCEXCEPTION_HPP
#define IRCEXCEPTION_HPP

#include <IRC.hpp>

namespace IRCException
{
	class IRCException : public std::exception
	{
	public:
		IRCException(const std::string &message);
		virtual ~IRCException() throw();

		virtual const char *what() const throw();

	private:
		String m_msg;
	};
}

#endif
// IRCEXCEPTION_HPP
