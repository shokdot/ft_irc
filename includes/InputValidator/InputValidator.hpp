#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <IRC.hpp>

class InputValidator
{
public:
	static std::pair<int, String> validateArgs(int, char *[]);

private:
	static int validatePort(const char *);
	static String validatePassword(const char *);
};

#endif
// INPUT_VALIDATOR_HPP
