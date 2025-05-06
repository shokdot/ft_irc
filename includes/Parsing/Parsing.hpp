#ifndef PARSING_HPP
#define PARSING_HPP

#include <IRC.hpp>

class Parsing
{
public:
	static std::pair<int, String> argsValidate(int, char *[]);

private:
	static int validatePort(char *);
	static String validatePassword(char *);
};

#endif
// PARSING_HPP
