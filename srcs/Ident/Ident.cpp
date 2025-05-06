#include <Ident.hpp>

String Ident::reverseDNS(struct sockaddr_in &clientAddr)
{

	const char *ip = inet_ntoa(clientAddr.sin_addr);
	struct addrinfo hints, *res = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(ip, NULL, &hints, &res) == 0 && res->ai_canonname)
	{
		std::string hn(res->ai_canonname);
		freeaddrinfo(res);
		return hn;
	}
	if (res)
		freeaddrinfo(res);
	return String(ip);
}

void Ident::identLookup(struct sockaddr_in &clientAddr, int serverPort)
{
	int indentSock = socket(AF_INET, SOCK_STREAM, 0);
	if (indentSock < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in identAddr = clientAddr;
	identAddr.sin_port = htons(113);
	if (connect(indentSock, (struct sockaddr *)&identAddr, sizeof(identAddr)) == 0)
	{
		char query[64];
		snprintf(query, sizeof(query), "%d , %d\r\n", ntohs(clientAddr.sin_port), serverPort);
		send(indentSock, query, strlen(query), 0);

		char response[256];
		recv(indentSock, response, sizeof(response) - 1, 0);
		printf("IDENT response: %s\n", response);
	}
	else
	{
		perror("IDENT connection failed");
	}
	close(indentSock);
}
