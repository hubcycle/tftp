
#include "packet.h"
#include "helper.h"
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
	// struct addrinfo hints, *servinfo;
	// struct sockaddr_in client_addr;
	// socklen_t addr_len;
	// int rv;
	// pid_t childpid;

	// bzero(&hints, sizeof(struct addrinfo));
	// hints.ai_family = AF_INET;
	// hints.ai_socktype = SOCK_DGRAM;
	// hints.ai_flags = AI_PASSIVE;

	// if ((rv = getaddrinfo(NULL, s_port, &hints, &servinfo)) != 0)
	// {
	// 	perror(gai_strerror(rv));
	// 	exit(EXIT_FAILURE);
	// }

	// for (struct addrinfo *itr = servinfo; itr != NULL; itr = itr->ai_next)
	// {
	// 	if ((sockfd = socket(itr->ai_family, itr->ai_socktype, itr->ai_protocol)) == -1)
	// 	{
	// 		continue;
	// 	}
	// 	if (bind(sockfd, itr->ai_addr, itr->ai_addrlen) == -1)
	// 	{
	// 		close(sockfd);
	// 		continue;
	// 	}
	// 	else
	// 	{
	// 		break; // Socket binding successful
	// 	}
	// }

	// freeaddrinfo(servinfo);

	printf("SERVER: waiting...\n");
	// fflush(stdout);

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

		create_string_to_packet(buffer, num_bytes_read, &packet);

		printf("SERVER: got packet from %s\n", inet_ntop(client_addr.sa_family, &client_addr.sa_data, s, sizeof(s)));
		printf("SERVER: packet is %d bytes in length\n", num_bytes_read);
		printf("SERVER: packet contains %s\n", buffer);

		if ((childpid = fork()) == 0)
		{
			close(sockfd);

			serve_client(client_addr, &packet);

			exit(0);
		}
		else
		{
			sleep(3);
		}
	}
}

void serve_client(struct sockaddr client_addr, PACKET *packet)
{
	int client_sockfd = make_udpsocket_bind(0);

	if (packet->opcode == OPCODE_RRQ)
	{
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