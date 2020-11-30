
#include "packet.h"
#include "transfer.h"

void serve_client(struct sockaddr client_addr, PACKET *packet);
void send_to_client(int sockfd, struct sockaddr *client_addr, PACKET *packet);

int main(int argc, char *argv[])
{
	int port = atoi(argv[1]);
	char s_port[10];
	strcpy(s_port, argv[1]);

	int sockfd;
	printf("Utilising port:\t%d\n", port);
	sockfd = make_udpsocket_bind(port);
	
	printf("SERVER: waiting...\n");

	struct sockaddr client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int num_bytes_read;
	char buffer[MAX_BUFFER], s[INET_ADDRSTRLEN];
	pid_t childpid;
	PACKET packet;

	while (1)
	{
		bzero(buffer, sizeof(buffer));

		if ((num_bytes_read = recvfrom(sockfd, buffer, sizeof(buffer), 0, &client_addr, &addr_len)) == -1)
		{
			perror("Server: recvfrom error");
			exit(EXIT_FAILURE);
		}
		buffer[num_bytes_read] = '\0';

		printf("%d\n", *buffer);
		fflush(stdout);

		create_string_to_packet(buffer, num_bytes_read, &packet);

		// print_packet(&packet);

		

		if ((childpid = fork()) == 0)
		{
			printf("SERVER: got packet from %s\n", inet_ntop(client_addr.sa_family, &client_addr.sa_data, s, sizeof(s)));
			printf("SERVER: packet is %d bytes in length\n", num_bytes_read);
			printf("SERVER: packet contains %s\n", buffer);

			close(sockfd);

			serve_client(client_addr, &packet);

			exit(0);
		}
		else
		{
			// close(sockfd);
			sleep(3);
		}
	}
}

void serve_client(struct sockaddr client_addr, PACKET *packet)
{
	int client_sockfd = make_udpsocket_bind(0);

	if (packet->opcode == OPCODE_RRQ)
	{
		printf("Fulfilling client's RRQ for file %s\n", packet->read_request.filename);
		send_to_client(client_sockfd, &client_addr, packet);
	}
	else if (packet->opcode == OPCODE_WRQ)
	{
		printf("Not supported\n");
	}
	else
	{
		send_error(client_sockfd, &client_addr, ERRCODE_INVALID_OPERATION, "invalid request");
	}
}

void send_to_client(int sockfd, struct sockaddr *client_addr, PACKET *packet)
{
	if (strncmp(packet->read_request.mode, "octet", 6) != 0)
	{
		send_error(sockfd, client_addr, ERRCODE_INVALID_OPERATION, "invalid mode");
	}

	FILE *file = fopen(packet->read_request.filename, "rb");

	if (file == NULL)
	{
		if (errno == ENOENT)
		{
			send_error(sockfd, client_addr, ERRCODE_FILE_NOT_FOUND, strerror(errno));
		}

		send_error(sockfd, client_addr, ERRCODE_FILE_ACCESS_DENIED, strerror(errno));
	}

	send_file_to_client(sockfd, client_addr, file);
}