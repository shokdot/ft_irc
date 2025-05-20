#ifndef UTILS_HPP
#define UTILS_HPP

#include <IRC.hpp>

struct Utils
{
public:
	static String rtrim(const String &str);
	static String ltrim(const String &str);
	static String trim(const String &str);
	static void sendWrapper(const String &str, int sock_fd);
	static struct sockaddr_in createSockStruct(sa_family_t family, in_port_t port, in_addr_t addr);
};

#endif
// UTILS_HPP
