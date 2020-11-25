#include "packet.h"

int wait_packet(int sockfd, struct sockaddr *client_addr, unsigned short opcode, PACKET *packet)
{
	char packet_buffer[MAX_BUFFER];
	size_t num_of_bytes;
	socklen_t client_length = sizeof(client_addr);

	errno = 0;
	while (1)
	{
		printf("Waiting for message..");
		num_of_bytes = recvfrom(sockfd, packet_buffer, sizeof(packet_buffer), 0, client_addr, &client_length);
		printf("Received\n");

		if (errno != 0)
		{
			if (errno == EINTR)
			{
				printf("timeout \n");
			}
			else
			{
				printf("Error %d\n", errno);
			}
			return -1;
		}

		if (create_string_to_packet(packet_buffer, num_of_bytes, packet) == NULL)
		{
			return -1;
		}

		if (packet->opcode == opcode)
		{
			return num_of_bytes;
		}
		else if (packet->opcode == OPCODE_ERR)
		{
			return 0;
		}
	}
}
