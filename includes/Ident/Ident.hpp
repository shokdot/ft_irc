#ifndef IDENT_HPP
#define IDENT_HPP

#include <IRC.hpp>

class Ident
{
public:
	static String reverseDNS(struct sockaddr_in &clientAddr);
	static void IdentLookup(struct sockaddr_in &clientAddr, int serverPort);
};

#endif
