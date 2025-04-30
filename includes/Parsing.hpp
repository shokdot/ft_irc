#ifndef PARSING_HPP
#define PARSING_HPP

#include <IRC.hpp>

class Parsing
{
public:
	static std::pair<int, String> args_validate(int, char *[]);

private:
	static int validate_port(char *);
	static String validate_password(char *);
};

#endif
// PARSING_HPP
