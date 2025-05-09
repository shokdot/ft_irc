#ifndef IDENT_HPP
#define IDENT_HPP

#include <IRC.hpp>

class IdentService
{
public:
	static String reverseDNS(struct sockaddr_in &clientAddr);
	static void identLookup(struct sockaddr_in &clientAddr, int serverPort);
};

#endif
