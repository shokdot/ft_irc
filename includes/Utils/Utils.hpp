#ifndef UTILS_HPP
#define UTILS_HPP

#include <IRC.hpp>

struct Utils
{
public:
	static String rtrim(const String &str);
	static String ltrim(const String &str);
	static String trim(const String &str);
	static String strToUpper(const String &str);
	static String strToLower(const String &str);
	static std::vector<String> splitByDelim(const String &line, const char delimeter);
	static struct sockaddr_in createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr);
	static struct pollfd createPollStruct(int fd, short events);
};

#endif
// UTILS_HPP
