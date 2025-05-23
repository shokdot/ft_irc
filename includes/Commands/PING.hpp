#ifndef PING_HPP
#define PING_HPP

#include <ACommand.hpp>

class PING : public ACommand
{
private:
	/* data */
public:
	PING(/* args */);
	~PING();
	void execute();
};

#endif
