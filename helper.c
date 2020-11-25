#include "helper.h"

int make_udpsocket_bind(int port)
{
	int sockfd;
	struct sockaddr_in server_addr;

	//create a socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd == -1)
	{
		return -1;
	}

	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		return -1;
	}

	return sockfd;
}