#include "helper.h"

int make_udpsocket_bind(int port)
{
	int sockfd;
	struct sockaddr_in server_addr;

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

short get_network_to_host_short(char *buffer)
{
	short data;
	memcpy(&data, buffer, sizeof(short));
	return ntohs(data);
}

short get_host_to_network_short(short host_short, char *buffer)
{
	short data;
	data = htons(host_short);
	if (buffer != NULL)
	{
		memcpy(buffer, &data, sizeof(short));
	}

	return data;
}

size_t copy_n_src_dest(char *dest, const char *src, size_t n)
{
	size_t i, length;

	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}

	length = i + 1;

	while (i < n)
	{
		dest[i++] = '\0';
	}

	return length;
}