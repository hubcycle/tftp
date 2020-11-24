#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

#define MAX_BUFFER 550
#define MAX_TRIES 3
#define TIMOUT 5

int main(int argc, char *argv[])
{
	// int port_number = atoi(argv[1]);

	// int master_socket, client_sockfd;
	// int master_len, client_len;
	// struct addrinfo *serverinfo;
	// struct addrinfo /* condition */ hints;

	// struct sockaddr_in server_addr;
	// struct sockaddr_in client_addr;
	// char buffer[MAX_BUFFER];
	// socklen_t address_length;
	

	// //	Server settings
	// memset(&hints, 0, sizeof(struct addrinfo));
	// hints.ai_socktype = SOCK_DGRAM;
	// hints.ai_family = AF_INET;
	
	// if ((master_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
	// {
	// 	perror("master socket failed");
	// 	exit(EXIT_FAILURE);
	// }

	// server_addr.sin_family = AF_INET;
	// server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// server_addr.sin_port = htons(port_number);
	// master_len = sizeof(server_addr);
	// bind(master_socket, (struct sockaddr *)&server_addr, master_len);

	// listen(master_socket, 3);
	// signal(SIGCHLD, SIG_IGN);

	

	// //	Server settings end

	// while (1)
	// {
	// 	char ch;
	// 	printf("server ready to accept request\n");

	// 	client_len = sizeof()
	// }

	int port_number = atoi(argv[1]);


	int listenfd, sockfd, maxfdp1, readyfd;
	char buffer[MAX_BUFFER];
	pid_t childpid;
	fd_set rset;
	ssize_t bytes_received;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in server_addr, client_addr;
	char *message = "Request received";

	printf("done\n");
	fflush(stdout);
	
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_number);
	server_addr.sin_family = AF_INET;

	bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	// listen(listenfd, 3);

	FD_ZERO(&rset);
	maxfdp1 = listenfd + 1;

	while (1)
	{ 
		FD_SET(listenfd, &rset);
		printf("set\n");
		fflush(stdout);

		readyfd = select(maxfdp1, &rset, NULL, NULL, NULL);

		printf("descriptors set\n");
		fflush(stdout);

		len = sizeof(client_addr);
		bzero(buffer, sizeof(buffer));
		printf("Request received:\n");
		bytes_received = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &len);
		buffer[bytes_received] = '\0';
		printf("%ld\t%s\n", bytes_received, buffer);
		fflush(stdout);
	}
}