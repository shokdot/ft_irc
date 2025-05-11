#ifndef INPUTVALIDATOR_HPP
#define INPUTVALIDATOR_HPP

#include <IRC.hpp>

class InputValidator
{
public:
	static std::pair<int, String> validateArgs(int, char *[]);

private:
	static int validatePort(char *);
	static String validatePassword(char *);
};

#endif
// INPUTVALIDATOR_HPP
