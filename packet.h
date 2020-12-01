#ifndef PACKET_H
#define PACKET_H

#include "helper.h"

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

void print_error(const PACKET *packet);

#endif