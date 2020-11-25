#ifndef PACKET_H
#define PACKET_H

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
#include <errno.h>
#include <stdbool.h>

#define MAX_BUFFER 550
#define MAX_FILENAME_LENGTH 256
#define MAX_PACKET_LENGTH 512
#define MAX_TRIES 3
#define TIMEOUT 5

#define MAX_STRING_SIZE 1024
#define MAX_MODE_SIZE 8
#define MAX_DATA_SIZE 512

#define OPCODE_RRQ 1
#define OPCODE_WRQ 2
#define OPCODE_DATA 3
#define OPCODE_ACK 4
#define OPCODE_ERR 5

#define ERRCODE_UNDEFINED 0
#define ERRCODE_FILE_NOT_FOUND 1
#define ERRCODE_FILE_ACCESS_DENIED 2
#define ERRCODE_INVALID_OPERATION 3
#define ERRCODE_UNKNOWN_BLOCK_ID 4

typedef struct
{
	char filename[MAX_STRING_SIZE + 1];
	char mode[MAX_MODE_SIZE + 1];
} RRQ, WRQ;

typedef struct
{
	unsigned short block_number;
	unsigned int data_size;
	char data[MAX_DATA_SIZE];
} DATA;

typedef struct
{
	unsigned short block_number;
} ACK;

typedef struct
{
	unsigned short error_code;
	char message[MAX_STRING_SIZE];
} ERROR;

typedef struct
{
	unsigned short opcode;
	union
	{
		RRQ read_request;
		DATA data;
		ACK ack;
		ERROR error;
	};
} PACKET;

PACKET *create_string_to_packet(char *buffer, size_t buffer_size, PACKET *packet);
size_t create_packet_to_string(const PACKET *packet, char *buffer);
int wait_packet(int sockfd, struct sockaddr *client_addr, unsigned short opcode, PACKET *packet);

void print_packet(PACKET *packet);

void print_error(PACKET *packet);

#endif