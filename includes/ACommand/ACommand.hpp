#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <IRC.hpp>

class ACommand
{
public:
	virtual ~ACommand();
	virtual void execute();
};

#endif
